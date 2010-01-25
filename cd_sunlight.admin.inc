<?php

/* 
 * @file
 * Admin menu callbacks for cd_sunlight
 */


/**
 * Admin settings form.
 */
function cd_sunlight_settings_form() {
  civicrm_initialize();
  $form = array();

  // Basic settings.
  $form['cd_sunlight_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Sunlight API Key'),
    '#description' => t('A key can be obtained from <a href="!url">Sunlight Labs</a>.'
      , array('!url' => url('http://services.sunlightlabs.com/api/'))),
    '#default_value' => variable_get('cd_sunlight_api_key', ''),
    '#required' => TRUE,
  );
  $form['cd_sunlight_cron_contact_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron contact limit'),
    '#description' => t('The maximum number of contacts to process during cron.'),
    '#default_value' => variable_get('cd_sunlight_cron_contact_limit', 300),
    '#required' => TRUE,
  );
  if (function_exists('sys_getloadavg')) {
    $form['cd_sunlight_cron_load_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Cron server load limit'),
      '#description' => t('Do not run Cron CD lookups if the system load is higher than this value. '
        . 'Only applicable to Linux based servers. '),
      '#default_value' => variable_get('cd_sunlight_cron_load_limit', 2),
    );
  }

  // Geocoding.
  $form['geocode'] = array(
    '#type' => 'fieldset',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#title' => 'Advanced Batch Geocoding',
    '#description' => t('<a href="!url_geocode">Geocoding within CiviCRM</a> should be enabled '
      . "for more accurate determination of a contact's congressional district (some congressional "
      . 'districts contain more than one zip code).  However geocoding can put a big load on the '
      . 'server if several contacts are saved per second from multiple HTTP requests. '
      . 'Instead CiviCRM Sunlight Congressional District has the ability to override CiviCRM and '
      . 'perform geocoding only in batches during cron and on any CiviCRM profiles used for '
      . '"View/Edit User Account".  To use this feature first <a href="!url_setup">setup '
      . ' a user, password and key for CiviCRM Command-line usage</a>. Enter these values below. '
      . '<br /><em>NOTE: 95% of sites will not require or see any benefit from batch geocoding.</em>'
      , array(
        '!url_geocode' => url('civicrm/admin/setting/mapping'),
        '!url_setup' => 'http://wiki.civicrm.org/confluence/display/CRMDOC/Command-line+Script+Configuration',
      )),
  );
  $form['geocode']['cd_sunlight_geocoding_off'] = array(
    '#type' => 'checkbox',
    '#title' => t('Run CiviCRM geocoding only during cron and some CiviCRM profiles'),
    '#default_value' => variable_get('cd_sunlight_geocoding_off', FALSE),
  );
  $form['geocode']['cd_sunlight_geocode_key'] = array(
    '#title' => t('CiviCRM command-line script key'),
    '#type' => 'item',
    '#value' => defined('CIVICRM_SITE_KEY') ? 'Is defined' : 'Is not defined',
    '#description' => t('The CiviCRM command-line key. '),
  );
  $form['geocode']['cd_sunlight_geocode_crypt'] = array(
    '#title' => t('Crypt libraries'),
    '#type' => 'item',
    '#value' => _cd_sunlight_crypt_enabled() ? 'Available' : 'Not available',
    '#description' => t('The <a href=!url>PHP MCrypt extension</a> is required for storing the '
      . 'username. ', array(
        '!url' => 'http://www.php.net/manual/en/book.mcrypt.php',
      )),
  );
  $form['geocode']['cd_sunlight_geocode_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username used in CiviCRM geocode script'),
    '#disabled' => !_cd_sunlight_crypt_enabled(),
    '#default_value' => _cd_sunlight_crypt_decrypt(variable_get('cd_sunlight_geocode_username', '')),
    '#description' => t('An administrator account should be avoided for security reasons.'),
    '#attributes' => array('autocomplete' => 'off'),
  );
  $form['geocode']['cd_sunlight_geocode_password'] = array(
    '#type' => 'item',
    '#title' => t('Password used in CiviCRM geocode script'),
    '#value' => defined('CD_SUNLIGHT_GEOCODE_PASSWORD') ? 'Is defined' : 'Is not defined',
    '#description' => t('Define the password in a constant called !const in settings.php',
      array('!const' => 'CD_SUNLIGHT_GEOCODE_PASSWORD')),
    '#attributes' => array('autocomplete' => 'off'),
  );
  $form['geocode']['http_auth'] = array(
    '#type' => 'fieldset',
    '#description' => t('If this site requires HTTP authentication.  Please enter the credentials '.
      'below'),
    '#title' => 'HTTP Authentication',
  );
  $form['geocode']['http_auth']['cd_sunlight_http_auth_user'] = array(
    '#type' => 'textfield',
    '#title' => 'User',
    '#disabled' => !_cd_sunlight_crypt_enabled(),
    '#default_value' => _cd_sunlight_crypt_decrypt(variable_get('cd_sunlight_http_auth_user', '')),
    '#attributes' => array('autocomplete' => 'off'),
  );
  $form['geocode']['http_auth']['cd_sunlight_http_auth_password'] = array(
    '#type' => 'textfield',
    '#title' => 'Password',
    '#disabled' => !_cd_sunlight_crypt_enabled(),
    '#default_value' => _cd_sunlight_crypt_decrypt(variable_get('cd_sunlight_http_auth_password', '')),
    '#attributes' => array('autocomplete' => 'off'),
  );
  $form['geocode']['test'] = array(
    '#type' => 'fieldset',
    '#description' => t('Any issues shown here, or in <a href="!url">the logs</a> (PHP timeouts, '.
        'etc.) must be resolved before batch geocoding will work. ', array(
        '!url' => url('admin/reports/dblog'),
      )),
  );
  $form['geocode']['test']['test'] = array(
    '#type' => 'submit',
    '#value' => t('Test batch geocoding'),
    '#submit' => array('cd_sunlight_contacts_geocode'),
    '#disabled' => !_cd_sunlight_crypt_enabled() ||
      !defined('CIVICRM_SITE_KEY') ||
      !defined('CD_SUNLIGHT_GEOCODE_PASSWORD') ||
      !variable_get('cd_sunlight_geocode_username', ''),
  );

  $form = system_settings_form($form);
  return $form;
}

function cd_sunlight_settings_form_validate($form, &$form_state) {

  // Temporarily replace the existing API key.
  $key_existing = variable_get('cd_sunlight_api_key', '');
  variable_set('cd_sunlight_api_key', $form_state['values']['cd_sunlight_api_key']);

  // Test the new one.
  if (!_cd_sunlight_connection_test()) {
    form_set_error('cd_sunlight_api_key'
      , t('Either the Sunlight key is not valid, or the Sunlight API is down. '
        .'See watchdog for more detailed information.'));
  }
  else {
    drupal_set_message(t('The Sunlight API key works.'));
  }

  // Revert.
  variable_set('cd_sunlight_api_key', $key_existing);

  // Are the contact limit and load limit numeric?
  if (!empty($form_state['values']['cd_sunlight_cron_contact_limit'])) {
    $contact_limit = $form_state['values']['cd_sunlight_cron_contact_limit'];
    if (!is_numeric($contact_limit) || (int)$contact_limit != $contact_limit) {
      form_set_error('cd_sunlight_cron_contact_limit',
        t('The contact limit must be an integer.'));
    }
  }
  if (!empty($form_state['values']['cd_sunlight_cron_load_limit'])) {
    $cron_limit = $form_state['values']['cd_sunlight_cron_load_limit'];
    if (!is_numeric($cron_limit) || $cron_limit < 0) {
      form_set_error('cd_sunlight_cron_load_limit',
        t('The cron limit must be a positive numeric value.'));
    }
  }

  // Test the geocoding username.
  if ($user_name = $form_state['values']['cd_sunlight_geocode_username']) {
    $message = 'The geocoding user does not exist';
    $user_valid = FALSE;
    $account = user_load(array('name' => $user_name));
    if ($account && $account->uid) {
      $message = 'The geocoding user exists';
      if (user_access('access all custom data', $account)) {
        $message .= ' with correct permissions';
        drupal_set_message($message);
        $user_valid = TRUE;
      }
      else {
        $message .= ' but without correct permissions';
      }
    }
    if (!$user_valid) {
      form_set_error('cd_sunlight_geocode_username', $message);
    }

    // Encrypt the geocoding username.
    if (_cd_sunlight_crypt_enabled()) {
      $user_name_encrypted =
        _cd_sunlight_crypt_encrypt($user_name);
      form_set_value($form['geocode']['cd_sunlight_geocode_username'],
        $user_name_encrypted, $form_state);
    }
  }

  // Encrypt HTTP Auth credentials.
  if ($http_user = $form_state['values']['cd_sunlight_http_auth_user']) {
    if (_cd_sunlight_crypt_enabled()) {
      $http_user_encrypted =
        _cd_sunlight_crypt_encrypt($http_user);
      form_set_value($form['geocode']['http_auth']['cd_sunlight_http_auth_user'],
        $http_user_encrypted, $form_state);
      $http_password_encrypted =
        _cd_sunlight_crypt_encrypt($form_state['values']['cd_sunlight_http_auth_password']);
      form_set_value($form['geocode']['http_auth']['cd_sunlight_http_auth_password'],
        $http_password_encrypted, $form_state);
    }
  }

}

/**
 * Form to choose CiviCRM custom fields that stores data.
 *
 * @return array
 *  FAPI array
 */
function cd_sunlight_settings_fields_form() {

  // Get options.
  db_set_active('civicrm');
  $resource = db_query('SELECT id, custom_group_id, label FROM civicrm_custom_field
    WHERE data_type = "string" ORDER BY custom_group_id, weight');
  db_set_active();
  $fields = array();
  while ($field = db_fetch_array($resource)) {
    $fields[$field['id']] = $field['custom_group_id'] .': '. check_plain($field['label']);
  }

  // Form.
  $form = array(
    '#prefix' => '<div>' . t(
      'CiviCRM Congressional District stores congressional district data for each CiviCRM '
      . 'contact in a set of CiviCRM custom fields.  Create these fields <a href="!url">within '
      . 'CiviCRM</a>. ', array('!url' => url('civicrm/admin/custom/group'))) .'</div>' ,
  );

  $field_cd_sunlight_civicrm_cd = variable_get('cd_sunlight_civicrm_cd', '');
  $field_cd_sunlight_civicrm_cd = explode('_', $field_cd_sunlight_civicrm_cd);
  $field_cd_sunlight_civicrm_cd = array_pop($field_cd_sunlight_civicrm_cd);
  $form['field_cd_sunlight_civicrm_cd'] = array(
    '#type' => 'select',
    '#title' => 'Congressional District Field',
    '#default_value' => check_plain($field_cd_sunlight_civicrm_cd),
    '#options' => $fields,
    '#description' => t('Create one alphanumeric text field (should be searchable and if you are '
      . 'using a version of CiviCRM where bug <a href="!CRM-586">CRM-5686</a> is fixed you '
      . 'probably want to make the field view-only) to store the contact\'s congressional district.',
      array('!CRM-586' => 'http://issues.civicrm.org/jira/browse/CRM-5686')),
    '#required' => TRUE,
  );

  $field_cd_sunlight_civicrm_cd_override = variable_get('cd_sunlight_civicrm_cd_override', '');
  $field_cd_sunlight_civicrm_cd_override = explode('_', $field_cd_sunlight_civicrm_cd_override);
  $field_cd_sunlight_civicrm_cd_override = array_pop($field_cd_sunlight_civicrm_cd_override);
  $form['field_cd_sunlight_civicrm_cd_override'] = array(
    '#type' => 'select',
    '#title' => 'Congressional District Override Field',
    '#default_value' => check_plain($field_cd_sunlight_civicrm_cd_override),
    '#options' => $fields,
    '#description' => t('Create a second alphanumeric text field (should be searchable) '
      . 'to allow administrators to manually override the value queried from Sunlight. '),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  return $form;

}

function cd_sunlight_settings_fields_form_validate($form, &$form_state) {

  // Check that both fields are in the same table.
  db_set_active('civicrm');
  $table_1 = db_result(db_query('SELECT custom_group_id FROM civicrm_custom_field
    WHERE id = %d', $form_state['values']['field_cd_sunlight_civicrm_cd']));
  $table_2 = db_result(db_query('SELECT custom_group_id FROM civicrm_custom_field
    WHERE id = %d', $form_state['values']['field_cd_sunlight_civicrm_cd_override']));
  db_set_active();
  if ($table_1 != $table_2) {
    form_set_error('field_cd_sunlight_civicrm_cd_override'
      , t('Choose two fields from the same group.'));
  }
}

function cd_sunlight_settings_fields_form_submit($form, &$form_state) {

  // Retrieve the fields data.
  db_set_active('civicrm');
  $field_cd_sunlight_civicrm_cd = db_fetch_object(db_query('SELECT f.id, f.column_name, g.table_name
    FROM civicrm_custom_field f
    INNER JOIN civicrm_custom_group g
      ON f.custom_group_id = g.id
    WHERE f.id = %d', $form_state['values']['field_cd_sunlight_civicrm_cd']));
  $field_cd_sunlight_civicrm_cd_override = db_fetch_object(db_query('SELECT f.id, f.column_name, g.table_name
    FROM civicrm_custom_field f
    INNER JOIN civicrm_custom_group g
      ON f.custom_group_id = g.id
    WHERE f.id = %d', $form_state['values']['field_cd_sunlight_civicrm_cd_override']));
  db_set_active();

  // Save the variables.
  variable_set('cd_sunlight_civicrm_custom_table',
    $field_cd_sunlight_civicrm_cd->table_name);
  variable_set('cd_sunlight_civicrm_cd',
    'custom_'. $field_cd_sunlight_civicrm_cd->id);
  variable_set('cd_sunlight_civicrm_cd_override',
    'custom_'. $field_cd_sunlight_civicrm_cd_override->id);
  variable_set('cd_sunlight_civicrm_custom_field_cd',
    $field_cd_sunlight_civicrm_cd->column_name);
  variable_set('cd_sunlight_civicrm_custom_field_cd_override',
    $field_cd_sunlight_civicrm_cd_override->column_name);

  drupal_set_message(t('The fields have been saved.'));
}

