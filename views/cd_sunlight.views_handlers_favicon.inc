<?php
// $Id$

/**
 * @file
 * Views handlers for cd_sunlight.
 */

/**
 * Field handler to provide simple renderer that turns a URL into a clickable link with
 * optional favicon.
 *
 * Is not useful on its own unless extended by a child class which sets the following
 * on render:
 *  $this->favicon_url
 *  $this->human_readable
 *  $this->url_path
 *  $this->url_query
 *
 * @see views_handler_field_url_favicon_youtube()
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon extends views_handler_field_url {
  
  protected $favicon_url;
  protected $human_readable;
  protected $url_path;
  protected $url_query;

  function option_definition() {
    $options = parent::option_definition();

    $options['display_as_favicon'] = array('default' => TRUE);
    $options['new_window'] = array('default' => TRUE);

    return $options;
  }

  /**
   * Provide link to the page being visited.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['display_as_favicon'] = array(
      '#title' => t('Display as favicon'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['display_as_favicon']),
    );
    $form['new_window'] = array(
      '#title' => t('Open link in a new window'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['new_window']),
    );
  }

  function render($values) {
    if (empty($values->{$this->field_alias})) {
      return NULL;
    }
    if (!empty($this->options['display_as_favicon']) && !empty($this->favicon_url)) {

      // Resolve favicons.
      if ($local_url = _cd_sunlight_remote_image_fetch($this->favicon_url)) {
        $value = theme('image', $local_url, '',
            $this->human_readable, array('height' => '16px', 'width' => '16px'), FALSE);
      }

    }
    if (!empty($this->options['display_as_link'])) {
      if (empty($value)) {
        $value = check_plain($this->human_readable);
      }
      $l_options = array('html' => TRUE, 'query' => $this->url_query);
      if (empty($this->options['display_as_favicon'])) {
        $l_options['attributes']['title'] = $this->human_readable;
      }
      if (!empty($this->options['new_window'])) {
        $l_options['attributes']['target'] = '_blank';
      }
      return l($value, $this->url_path, $l_options);
    }
    else {
      return check_plain($this->url_path . ($this->url_query ? '?' : '') . $this->url_query);
    }
  }

}

/**
 * Field handler to provide favicon for YouTube URL.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_youtube extends views_handler_field_url_favicon {

  function render($values) {
    $this->favicon_url = 'http://www.youtube.com/favicon.ico';
    $this->human_readable = 'YouTube';
    $this->url_path = $values->{$this->field_alias};
    $this->url_query = NULL;
    return parent::render($values);
  }

}

/**
 * Field handler to provide favicon for Bioguide.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_bioguide extends views_handler_field_url_favicon {

  function render($values) {
    $this->favicon_url = 'http://bioguide.congress.gov/favicon.ico';
    $this->human_readable = 'Bioguide';
    $this->url_path = 'http://bioguide.congress.gov/scripts/biodisplay.pl';
    $this->url_query = 'index='. $values->{$this->field_alias};
    return parent::render($values);
  }

}

/**
 * Field handler to provide favicon for Project Vote Smart.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_votesmart extends views_handler_field_url_favicon {

  function render($values) {
    $this->favicon_url = 'http://votesmart.org/favicon.ico';
    $this->human_readable = 'Project Vote Smart';
    $this->url_path = 'http://votesmart.org/bio.php';
    $this->url_query = 'can_id='. $values->{$this->field_alias};
    return parent::render($values);
  }

}

/**
 * Field handler to provide favicon for Govtrack.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_govtrack extends views_handler_field_url_favicon {

  function render($values) {
    $this->favicon_url = 'http://www.govtrack.us/favicon.ico';
    $this->human_readable = 'Govtrack';
    $this->url_path = 'http://govtrack.us/congress/person.xpd';
    $this->url_query = 'id='. $values->{$this->field_alias};
    return parent::render($values);
  }

}

/**
 * Field handler to provide favicon for OpenSecrets.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_opensecrets extends views_handler_field_url_favicon {

  function render($values) {
    $this->favicon_url = 'http://www.opensecrets.org/img/favicon.ico';
    $this->human_readable = 'OpenSecrets';
    $this->url_path = 'http://www.opensecrets.org/politicians/summary.php';
    $this->url_query = 'cid='. $values->{$this->field_alias};
    return parent::render($values);
  }

}

/**
 * Field handler to provide favicon for OpenCongress.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_congresspedia extends views_handler_field_url_favicon {

  function render($values) {
    $this->favicon_url = 'http://www.opencongress.org/favicon.ico';
    $this->human_readable = 'OpenCongress';
    $this->url_path = $values->{$this->field_alias};
    $this->url_query = NULL;
    return parent::render($values);
  }

}

/**
 * Field handler to provide favicon for Twitter.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_twitter extends views_handler_field_url_favicon {

  function render($values) {
    $this->favicon_url = 'http://twitter.com/favicon.ico';
    $this->human_readable = 'Twitter';
    $this->url_path = 'http://twitter.com/'. $values->{$this->field_alias};
    $this->url_query = NULL;
    return parent::render($values);
  }

}

/**
 * Field handler to provide favicon for Progressive Punch.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_progressive_punch extends views_handler_field_url_favicon {

  /**
   * Add all the fields that we need.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['state'] = 'state';
    $this->additional_fields['district'] = 'district';
    $this->additional_fields['title'] = 'title';
    $this->additional_fields['senate_class'] = 'senate_class';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $this->favicon_url = 'http://progressivepunch.org/images/boxer_l2.jpg';
    $this->human_readable = 'Progressive Punch';
    $this->url_path = 'http://progressivepunch.org/members.jsp';

    // Their URL structure sucks!  For reps it's fairly easy.
    // For sens we need to use a more general call.
    $query = array(
      'state' => $values->{$this->aliases['state']},
    );
    if ($values->{$this->aliases['title']} == 'Rep') {
      $query['district'] = $values->{$this->aliases['district']};
    }
    elseif ($senate_class = $values->{$this->aliases['senate_class']}) {
      $query['member'] = $values->{$this->aliases['state']} . $senate_class;
    }
    $this->url_query = drupal_query_string_encode($query);

    // parent::render checks if any data exists in the field.  We circumvent this check:
    $values->{$this->field_alias} = TRUE;

    return parent::render($values);
  }

}

/**
 * Field handler to provide favicon for Generic URLs.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_generic extends views_handler_field_url_favicon {

  function render($values) {
    if (empty($this->human_readable)) {
      $this->human_readable = '';
    }
    $url = parse_url($values->{$this->field_alias});
    // Check for a bad URL.
    if (empty($url['host'])) {
      $this->favicon_url = '';
      $this->url_path = $values->{$this->field_alias};
      $this->url_query = NULL;
    }
    else {
      if (empty($url['scheme'])) {
        $url['scheme'] = 'http';
      }
      if (empty($url['path'])) {
        $url['path'] = '/';
      }
      $this->favicon_url = $url['scheme'] .'://'. $url['host'] .'/favicon.ico';
      $this->url_path = $url['scheme'] .'://'. $url['host'] .''. $url['path'];
      $this->url_query = isset($url['query']) ? $url['query'] : NULL;
    }
    return parent::render($values);
  }

}

/**
 * Field handler to provide favicon for Webforms.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_webform extends views_handler_field_url_favicon_generic {

  function render($values) {
    $this->human_readable = 'Contact form';
    return parent::render($values);
  }

}

/**
 * Field handler to provide favicon for Webforms.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url_favicon_website extends views_handler_field_url_favicon_generic {

  function render($values) {
    $this->human_readable = 'Website';
    return parent::render($values);
  }

}