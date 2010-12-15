<?php
// $Id$

/**
 * Filter handler for state.  This is very basic, but cd_civicrm can override it.
 */
class cd_sunlight_handler_filter_district extends views_handler_filter_many_to_one {

  function get_value_options() {
    if (!isset($this->value_options)) {
      $result = db_query('SELECT district FROM {cd_sunlight_legislators} GROUP BY district ORDER BY district');
      while ($district = db_fetch_object($result)) {
        $this->value_options[$district->district] = $district->district;
      }
    }
  }

  function query() {
    parent::query();
    
    // TODO: This is not working.

    // We need to move the district clause into its own WHERE group so that we can add an OR
    // for senators.
    foreach ($this->query->where[0]['clauses'] as $clause_id => $clause) {
      if (strpos($clause, '.district ') !== FALSE) {
        $district_clause_id = $clause_id;
        break;
      }
    }

    // Add our new WHERE group.
    if (!empty($district_clause_id)) {

      // Remove the old clause.
      $district = $this->query->where[0]['args'][$district_clause_id];
      $where = $this->query->where[0]['clauses'][$district_clause_id];
      unset($this->query->where[0]['clauses'][$district_clause_id],
        $this->query->where[0]['args'][$district_clause_id]);

      // Add a new group.
      $group = $this->query->set_where_group('OR');
      $this->query->add_where($group, $where, $district);

      // Also show senators for this CD.
      $this->query->add_where($group, "$this->table_alias.$this->real_field = 0");

    }

  }
}
