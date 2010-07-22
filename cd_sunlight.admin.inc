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
    '#default_value' => variable_get('cd_sunlight_cron_contact_limit', 200),
    '#required' => TRUE,
  );
  $form['cd_sunlight_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debugging'),
    '#description' => t('Extra log entries will be made to aid in debugging.'),
    '#default_value' => variable_get('cd_sunlight_debug', FALSE),
  );
  if (function_exists('sys_getloadavg')) {
    $form['cd_sunlight_cron_load_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Cron server load limit'),
      '#description' => t('Do not run Cron CD lookups if the system load is higher than this value. '
        . 'Only applicable to Linux based servers. '),
      '#default_value' => variable_get('cd_sunlight_cron_load_limit', 6),
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
    '#prefix' => '<p>' . t(
      'CiviCRM Congressional District stores congressional district data for each CiviCRM '
      . 'contact in a set of CiviCRM custom fields.  First create these fields <a href="!url">within '
      . 'CiviCRM</a> according to the descriptions below. ',
      array('!url' => url('civicrm/admin/custom/group'))) .'</p>' ,
  );

  $field_cd_sunlight_civicrm_cd = variable_get('cd_sunlight_civicrm_cd', '');
  $field_cd_sunlight_civicrm_cd = explode('_', $field_cd_sunlight_civicrm_cd);
  $field_cd_sunlight_civicrm_cd = array_pop($field_cd_sunlight_civicrm_cd);
  $form['field_cd_sunlight_civicrm_cd'] = array(
    '#type' => 'select',
    '#title' => 'Congressional District Field',
    '#default_value' => check_plain($field_cd_sunlight_civicrm_cd),
    '#options' => $fields,
    '#description' => t('Create one alphanumeric text field to store the contact\'s congressional '
      . 'district (should be searchable. If you are '
      . 'using a version of CiviCRM where bug <a href="!CRM-586">CRM-5686</a> is fixed you '
      . 'probably want to make the field view-only).',
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
    '#description' => t('Create a second alphanumeric text field to allow administrators to '
      . 'manually override the value queried from Sunlight (should be searchable). '),
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

/**
 * Confirm that we will batch process all contacts.
 */
function cd_sunlight_batch_confirm() {
  return confirm_form(array(),
    t('Process all contacts'),
    'admin/settings/cd_sunlight',
    t('Are you sure you want to process all contacts, queuing those that require a Congressional '.
      'District? This action iterates through all contacts in CiviCRM, and may be a lengthy process.'),
    t('Process all contacts'),
    t('Cancel')
  );
}

/**
 * Handler for batch confirmation.
 */
function cd_sunlight_batch_confirm_submit($form, &$form_state) {
  $batch = array(
    'operations' => array(array('cd_sunlight_batch', array())),
    'finished' => 'cd_sunlight_batch_finished',
    'title' => t('Processing CiviCRM contacts.'),
    'init_message' => t('Starting.'),
    'progress_message' => '',
    'error_message' => t('An error occurred and some contacts have not been processed.'),
    'file' => drupal_get_path('module', 'cd_sunlight') .'/cd_sunlight.admin.inc',
  );
  batch_set($batch);
}

/**
 * Batch process all CiviCRM contacts using Batch API.
 */
function cd_sunlight_batch(&$context) {

  // First time setup.
  static $limit = 50;
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['contact_id'] = 0;
    db_set_active('civicrm');
    $context['sandbox']['max'] = db_result(db_query('SELECT COUNT(*) FROM civicrm_contact'));
    db_set_active();
  }

  // Iterate through the contacts.
  db_set_active('civicrm');
  $res = db_query('SELECT id
    FROM civicrm_contact
    WHERE id > %d
    ORDER BY id LIMIT 0, '. $limit,
    $context['sandbox']['contact_id']);
  db_set_active();
  while ($contact = db_fetch_array($res)) {
    $contact = _cd_sunlight_civicrm_contact_get(array('contact_id' => $contact['id']));
    if (civicrm_error($contact)) {
      $context['success'] = FALSE;
      $context['error_message'] = t('There was a CiviCRM API error. ');
      return;
    }
    cd_sunlight_validate_civicrm_data($contact);
    $context['sandbox']['contact_id'] = $contact['contact_id'];
    $context['sandbox']['progress'] ++;
  }

  // Are we finished?
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  $context['message'] = t('Processed @progress of @max',
    array('@progress' => $context['sandbox']['progress'], '@max' => $context['sandbox']['max']));
  $context['results'] = $context['sandbox']['progress'];

}

function cd_sunlight_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural($results, 'One contact processed.', '@count contacts processed.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}

/**
 * Menu Callback to display {cd_sunlight_cron}.
 */
function cd_sunlight_log() {
  $rows = array();
  $header = array(
    array(
      'data' => t('Time'),
      'field' => 'insert_time',
      'sort' => 'DESC',
    ),
    array(
      'data' => t('Contact'),
      'field' => 'contact_id',
    ),
    array(
      'data' => t('Processed'),
      'field' => 'processed',
    ),
    array(
      'data' => t('URI'),
      'field' => 'request_uri',
    ),
  );
  if (variable_get('cd_sunlight_debug', FALSE)) {
    $header[] = t('Backtrace');
  }

  // Build data.
  $res = pager_query('SELECT * FROM {cd_sunlight_cron} '. tablesort_sql($header), 50);
  while ($record = db_fetch_object($res)) {
    $row = array(
      format_date($record->insert_time),
      l($record->contact_id, 'civicrm/contact/view', array('query' => 'reset=1&cid='. $record->contact_id)),
      $record->processed,
      check_plain($record->request_uri),
    );
    if (variable_get('cd_sunlight_debug', FALSE)) {
      $row[] = '<pre>'. check_plain($record->backtrace) .'</pre>';
    }
    $rows[] = $row;
  }
  
  // Create output;
  $output = theme('table', $header, $rows);
  $output .= theme('pager', array(), 50);
  return $output;
}

/**
 * Menu Callback to display summary analysis of {cd_sunlight_cron}.
 */
function cd_sunlight_log_summary() {
  $output = '';

  // Totals.
  $first_record = format_date(db_result(db_query('SELECT MIN(insert_time) FROM {cd_sunlight_cron}')));
  $last_record = format_date(db_result(db_query('SELECT MAX(insert_time) FROM {cd_sunlight_cron}')));
  $output .= '<p>'. t('Log contains data from : %first_record to %last_record',
      array('%first_record' => $first_record, '%last_record' => $last_record)) .'</p>';
  $total_unprocessed = db_result(db_query('SELECT COUNT(*) FROM {cd_sunlight_cron} WHERE processed = 0'));
  $total_processed = db_result(db_query('SELECT COUNT(*) FROM {cd_sunlight_cron} WHERE processed = 1'));
  $output .= '<p>'. t('Total records in the queue: %count',
    array('%count' => number_format($total_processed + $total_unprocessed))) .'</p>';
  $output .= '<p>'. t('Total records that have been processed: %count',
    array('%count' => number_format($total_processed))) .'</p>';
  $output .= '<p>'. t('Total records that have not been processed: %count',
    array('%count' => number_format($total_unprocessed))) .'</p>';

  // Most common contacts.
  $header = array('Contact', 'Count');
  $rows = array();
  $result = db_query('SELECT contact_id, COUNT(*) AS cnt
    FROM {cd_sunlight_cron}
    GROUP BY contact_id
    ORDER BY cnt DESC
    LIMIT 10');
  while ($record = db_fetch_object($result)) {
    $row = array(
      l($record->contact_id, 'civicrm/contact/view', array('query' => 'reset=1&cid='. $record->contact_id)),
      $record->cnt,
    );
    $rows[] = $row;
  }
  $output .= theme('table', $header, $rows, array(), t('Most active contacts'));

  // Most common URIs.
  $header = array('URI', 'Count');
  $rows = array();
  $result = db_query('SELECT request_uri, COUNT(*) AS cnt
    FROM {cd_sunlight_cron}
    GROUP BY request_uri
    ORDER BY cnt DESC
    LIMIT 10');
  while ($record = db_fetch_object($result)) {
    $row = array(
      $record->request_uri,
      $record->cnt,
    );
    $rows[] = $row;
  }
  $output .= theme('table', $header, $rows, array(), t('Most common URIs'));

  // Most active periods of time.
  // 1NN seconds => left trim length of unix timestamp
  $time_periods = array(
    '00' => 8,
    '000' => 7,
    '0000' => 6,
    '00000' => 5,
  );
  foreach ($time_periods as $seconds => $left_trim) {
    $header = array('Period', 'Queued contacts');
    $rows = array();
    $result = db_query('SELECT LEFT(insert_time, %d) AS period, COUNT(*) AS cnt
      FROM {cd_sunlight_cron}
      GROUP BY period
      ORDER BY cnt DESC
      LIMIT 10', $left_trim);
    while ($record = db_fetch_object($result)) {
      $start = format_date($record->period . $seconds);
      $end = format_date(($record->period + 1) . $seconds);
      $row = array(
        $start .' - '. $end,
        $record->cnt,
      );
      $rows[] = $row;
    }
    $output .= theme('table', $header, $rows, array(),
      t('Most active @seconds second period', array('@seconds' => number_format('1'. $seconds))));
  }

  return $output;
}
