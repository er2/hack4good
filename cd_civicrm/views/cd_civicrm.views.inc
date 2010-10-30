<?php
// $Id$

/*
 * @file
 * Views stuff for cd_civicrm.
 */

/**
 * Implementation of hook_views_data_alter().
 */
function cd_civicrm_views_data_alter(&$data) {
  $data['cd_sunlight_legislators']['state_current'] = array(
    'real field' => 'state',
    'title' => t("Current User's State"),
    'help' => t('Filter the view to the State of the currently logged in user.'),
    'filter' => array(
      'handler' => 'views_handler_filter_state_current_user',
      'type' => 'yes-no',
    ),
  );
  $data['cd_sunlight_legislators']['district_current'] = array(
    'real field' => 'district',
    'title' => t("Current User's District"),
    'help' => t('Filter the view to the District of the currently logged in user.
              Only useful when used with the %current_state filter.',
              array('%current_state' => t("Current User's State"))),
    'filter' => array(
      'handler' => 'views_handler_filter_district_current_user',
      'type' => NULL,
    ),
  );
}

/**
 * Implements hook_views_default_views_alter().
 */
function cd_civicrm_views_default_views_alter(&$views) {
  if (isset($views['Congress'])) {
    $view =& $views['Congress'];
    $handler = $view->new_display('block', 'Block - Current user', 'block_3');
    $handler->override_option('filters', array(
      'district_current' => array(
        'operator' => '=',
        'value' => '1',
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'district_current',
        'table' => 'cd_sunlight_legislators',
        'field' => 'district_current',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
      'state_current' => array(
        'operator' => '=',
        'value' => '1',
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'state_current',
        'table' => 'cd_sunlight_legislators',
        'field' => 'state_current',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
    ));
    $handler->override_option('block_description', '');
    $handler->override_option('block_caching', -1);
    $handler = $view->new_display('page', 'Page - Current user', 'page_2');
    $handler->override_option('filters', array(
      'district_current' => array(
        'operator' => '=',
        'value' => '',
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'district_current',
        'table' => 'cd_sunlight_legislators',
        'field' => 'district_current',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
      'state_current' => array(
        'operator' => '=',
        'value' => '1',
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'state_current',
        'table' => 'cd_sunlight_legislators',
        'field' => 'state_current',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
    ));
    $handler->override_option('path', 'my-congress');
    $handler->override_option('menu', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));
  }
}

/**
 * Implementation of hook_views_handlers().
 */
function cd_civicrm_views_handlers() {
  $path = drupal_get_path('module', 'cd_civicrm') .'/views';
  return array(
    'handlers' => array(
      'views_handler_filter_state_current_user' => array(
        'parent' => 'views_handler_filter_boolean_operator',
        'file' => 'cd_civicrm.views_handler_filter_current_user.inc',
        'path' => $path,
      ),
      'views_handler_filter_district_current_user' => array(
        'parent' => 'views_handler_filter_boolean_operator',
        'file' => 'cd_civicrm.views_handler_filter_current_user.inc',
        'path' => $path,
      ),
    ),
  );
}

/**
 * Implementation of hook_views_query_substitutions().
 */
function cd_sunlight_views_query_substitutions($view) {
  global $user;
  $substitutions = array();
  if ($view->base_table == 'cd_sunlight_legislators') {
    $substitutions['***CURRENT_USERS_STATE***'] = 'NULL';
    $substitutions['***CURRENT_USERS_DISTRICT***'] = 'NULL';
    if ($user->uid) {
      $cd = cd_civicrm_user_get_cd($user->uid);
      if ($cd && $cd != 'not found') {
        $substitutions['***CURRENT_USERS_STATE***'] = "'". cd_sunlight_cd_parse_state($cd) ."'";
        $substitutions['***CURRENT_USERS_DISTRICT***'] = intval(cd_sunlight_cd_parse_district($cd));
      }
    }
  }
  return $substitutions;
}

