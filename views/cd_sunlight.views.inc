<?php
// $Id$

/*
 * @file
 * Views stuff for cd_sunlight.
 */

/**
 * Implementation of hook_views_data().
 */
function cd_sunlight_views_data() {
  $data['cd_sunlight_legislators']['table']['group'] = t('Congress');
  $data['cd_sunlight_legislators']['table']['base'] = array(
    'field' => 'legislator_id',
    'title' => t('Congress'),
    'help' => t("The current members of Congress as returned by the Sunlight API."),
  );
  $data['cd_sunlight_legislators']['firstname'] = array(
    'title' => t('First Name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['middlename'] = array(
    'title' => t('Middle Name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['lastname'] = array(
    'title' => t('Last Name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['name_suffix'] = array(
    'title' => t('Name Suffix'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['nickname'] = array(
    'title' => t('Nickname'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['title'] = array(
    'title' => t('Title'),
    'help' => t('Usually "Sen" or "Rep".'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['cd_sunlight_legislators']['chamber'] = array(
    'real field' => 'title',
    'title' => t('Chamber'),
    'help' => t('"Senate" or "House".'),
    'field' => array(
      'handler' => 'views_handler_field_chamber',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['party'] = array(
    'title' => t('Party'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['cd_sunlight_legislators']['state'] = array(
    'title' => t('State'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['cd_sunlight_legislators']['state_current'] = array(
    'real field' => 'state',
    'title' => t("Current User's State"),
    'help' => t('Filter the view to the State of the currently logged in user.'),
    'filter' => array(
      'handler' => 'views_handler_filter_state_current_user',
      'type' => 'yes-no',
    ),
  );
  $data['cd_sunlight_legislators']['district'] = array(
    'title' => t('District'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
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
  $data['cd_sunlight_legislators']['gender'] = array(
    'title' => t('Gender'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['phone'] = array(
    'title' => t('Phone'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['fax'] = array(
    'title' => t('Fax'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['website'] = array(
    'title' => t('Website'),
    'field' => array(
      'handler' => 'views_handler_field_url_favicon_website',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['webform'] = array(
    'title' => t('Webform'),
    'field' => array(
      'handler' => 'views_handler_field_url_favicon_webform',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['email'] = array(
    'title' => t('Email'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['congress_office'] = array(
    'title' => t('Congress Office'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['bioguide_id'] = array(
    'title' => t('BioGuide page'),
    'help' => t('http://bioguide.congress.gov/'),
    'field' => array(
      'handler' => 'views_handler_field_url_favicon_bioguide',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['votesmart_id'] = array(
    'title' => t('VoteSmart page'),
    'help' => t('http://votesmart.org/'),
    'field' => array(
      'handler' => 'views_handler_field_url_favicon_votesmart',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['fec_id'] = array(
    'title' => t('FEC ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['govtrack_id'] = array(
    'title' => t('Govtrack page'),
    'help' => t('http://govtrack.us/'),
    'field' => array(
      'handler' => 'views_handler_field_url_favicon_govtrack',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['crp_id'] = array(
    'title' => t('OpenSecrets page'),
    'help' => t('http://www.opensecrets.org/'),
    'field' => array(
      'handler' => 'views_handler_field_url_favicon_opensecrets',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['congresspedia_url'] = array(
    'title' => t('OpenCongress page'),
    'help' => t('http://www.opencongress.org/'),
    'field' => array(
      'handler' => 'views_handler_field_url_favicon_congresspedia',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['twitter_id'] = array(
    'title' => t('Twitter Page'),
    'help' => t('http://twitter.com/'),
    'field' => array(
      'handler' => 'views_handler_field_url_favicon_twitter',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['youtube_url'] = array(
    'title' => t('YouTube Page'),
    'help' => t('http://youtube.com/'),
    'field' => array(
      'handler' => 'views_handler_field_url_favicon_youtube',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['picture'] = array(
    'field' => array(
      'title' => t('Picture'),
      'help' => t('Profile picture.'),
      'handler' => 'views_handler_field_congress_picture',
    ),
  );
  $data['cd_sunlight_legislators']['progressive_punch'] = array(
    'field' => array(
      'title' => t('Progressive Punch Page'),
      'help' => t('http://progressivepunch.org'),
      'handler' => 'views_handler_field_url_favicon_progressive_punch',
    ),

  );

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function cd_sunlight_views_handlers() {
  $path = drupal_get_path('module', 'cd_sunlight') .'/views';
  return array(
    'handlers' => array(
      'views_handler_field_congress_picture' => array(
        'parent' => 'views_handler_field',
        'file' => 'cd_sunlight.views_handler_field_congress_picture.inc',
        'path' => $path,
      ),
      'views_handler_filter_state_current_user' => array(
        'parent' => 'views_handler_filter_boolean_operator',
        'file' => 'cd_sunlight.views_handler_filter_current_user.inc',
        'path' => $path,
      ),
      'views_handler_filter_district_current_user' => array(
        'parent' => 'views_handler_filter_boolean_operator',
        'file' => 'cd_sunlight.views_handler_filter_current_user.inc',
        'path' => $path,
      ),
      'views_handler_field_chamber' => array(
        'parent' => 'views_handler_field',
        'file' => 'cd_sunlight.views_handler_field_chamber.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon' => array(
        'parent' => 'views_handler_field_url',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_youtube' => array(
        'parent' => 'views_handler_field_url_favicon',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_bioguide' => array(
        'parent' => 'views_handler_field_url_favicon',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_votesmart' => array(
        'parent' => 'views_handler_field_url_favicon',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_govtrack' => array(
        'parent' => 'views_handler_field_url_favicon',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_opensecrets' => array(
        'parent' => 'views_handler_field_url_favicon',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_congresspedia' => array(
        'parent' => 'views_handler_field_url_favicon',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_twitter' => array(
        'parent' => 'views_handler_field_url_favicon',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_generic' => array(
        'parent' => 'views_handler_field_url_favicon',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_webform' => array(
        'parent' => 'views_handler_field_url_favicon_generic',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_website' => array(
        'parent' => 'views_handler_field_url_favicon_generic',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
        'path' => $path,
      ),
      'views_handler_field_url_favicon_progressive_punch' => array(
        'parent' => 'views_handler_field_url_favicon',
        'file' => 'cd_sunlight.views_handlers_favicon.inc',
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
      $cd = cd_sunlight_user_get_cd($user->uid);
      if ($cd && $cd != 'not found') {
        $substitutions['***CURRENT_USERS_STATE***'] = "'". cd_sunlight_cd_parse_state($cd) ."'";
        $substitutions['***CURRENT_USERS_DISTRICT***'] = intval(cd_sunlight_cd_parse_district($cd));
      }
    }
  }
  return $substitutions;
}

