<?php
// $Id$

/**
 * Filter handler for state.  This is very basic, but cd_civicrm can override it.
 */
class cd_sunlight_handler_filter_state extends views_handler_filter_many_to_one {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $result = db_query('SELECT state FROM {cd_sunlight_legislators} GROUP BY state ORDER BY state');
      while ($state = db_fetch_object($result)) {
        $this->value_options[$state->state] = $state->state;
      }
    }
  }
}
