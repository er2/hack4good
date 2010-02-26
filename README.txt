$Id$

*** CIVICRM SUNLIGHT CONGRESSIONAL DISTRICTS ***


* ABOUT *

This module provides a way to automatically get congressional district information about your
contacts into CiviCRM using the Sunlight API.
http://wiki.sunlightlabs.com/Sunlight_API_Documentation
It also fetches data about the current members of congress.  You can access all this data via an
API.


* INSTALL *

1)Configure the module at /admin/settings/cd_sunlight
  Specifically the Sunlight API key.

2)Link with CiviCRM fields as /admin/settings/cd_sunlight/fields

3)Make sure you have an entry in db_url for civicrm in your settings.php file.  Something like:
    $db_url = array(
      'default' => 'mysqli://user:pass@localhost/drupal',
      'civicrm' => 'mysqli://user:pass@localhost/civicrm',
    );
  Make sure to use the same schema for both database ULRs (mysqli, mysql, pgsql).

4)Navigate to /admin/reports/status to see if the module is reporting any problems.

5)Batch process all contacts at /admin/settings/cd_sunlight/batch

6)Edit a contact and change the postal code to enqueue the contact for lookup.  The Congressional
  District will be retrieved the next time cron is run.


* USING THE API *

Here are the functions.  See the docbook comments for more info.
cd_sunlight_contact_enqueue($contact_id) 
cd_sunlight_contact_dequeue($contact_id) 
cd_sunlight_contact_get_cd($contact_id) 
cd_sunlight_contact_get_state($contact_id) 
cd_sunlight_contact_get_district($contact_id)
cd_sunlight_contacts_geocode($number_of_contacts = 300)
cd_sunlight_user_get_cd($uid) 
cd_sunlight_user_get_state($uid)
cd_sunlight_user_get_district($uid)
cd_sunlight_cd_load($cd, $return_properties = array('legislators')) 
cd_sunlight_cd_all() 
cd_sunlight_legislators_get($params = array(), $order_by = array('state' => 'ASC', 'district' => 'ASC', 'lastname' => 'ASC'))
cd_sunlight_validate_civicrm_data($contact)
cd_sunlight_cd_parse_state($cd)
cd_sunlight_cd_parse_district($cd)


* CONGRESS MODULE *

The congress module is an example implementation of the API.  It provides a few blocks and a user
tab showing members of congress. 


* DEVELOPED BY *

Advomatic LLC
http://advomatic.com


* SPONSORED BY *

Democrats.com
http://democrats.com
