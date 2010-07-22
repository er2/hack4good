<?php

/**
 * @file
 * Views handlers for cd_sunlight.
 */

/**
 * Field handler to fetch the congress member's photo from Bioguide.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_congress_picture extends views_handler_field {

  /**
   * Add all the fields that we need.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['bioguide_id'] = 'bioguide_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if (empty($values->{$this->aliases['bioguide_id']})) {
      return '';
    }

    $bioguide_id = $values->{$this->aliases['bioguide_id']};
    $letter = drupal_substr($bioguide_id, 0, 1);
    $images_url = "http://bioguide.congress.gov/bioguide/photo/$letter/$bioguide_id.jpg";

    // Fetch image locally.
    if ($local_url = _cd_sunlight_remote_image_fetch($images_url)) {
      return theme('image', $local_url);
    }
    else {
      return theme('image', $images_url);
    }

  }

}
