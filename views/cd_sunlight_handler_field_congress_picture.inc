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
class cd_sunlight_handler_field_congress_picture extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    if (module_exists('imagecache')) {
      $formats = array('' => '- None -');
      foreach (imagecache_presets() as $preset) {
        $formats[$preset['presetname']] = $preset['presetname'];
      }
      $form['format'] = array(
        '#title' => t('Format'),
        '#type' => 'select',
        '#default_value' => !empty($this->options['format']),
        '#options' => $formats,
      );
    }
  }

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
    if (!empty($this->options['format']) && function_exists('theme_imagecache')) {
      return theme('imagecache', $this->options['format'], $image_url);
    }
    else {
      return theme('image', $image_url);
    }
  }

}
