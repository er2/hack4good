<?php
// $Id$

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

    $image_url = cd_sunlight_bioguide_image_url($values->{$this->aliases['bioguide_id']});
    return theme('image', $image_url);
  }

}
