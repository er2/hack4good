$Id$

*** CIVICRM SUNLIGHT CONGRESSIONAL DISTRICTS ***


* ABOUT *
This module provides a way to automatically get congressional district information about your
contacts into CiviCRM using the Sunlight API.
http://wiki.sunlightlabs.com/Sunlight_API_Documentation
It also fetches data about the current members of congress.  You can access all this data via an
API.


* INSTALL *
1)Configure the module at admin/settings/cd_sunlight
2)Make sure you have an entry in db_url for civicrm in your settings.php file.  Something like:
  $db_url = array(
    'default' => 'mysqli://user:pass@localhost/drupal',
    'civicrm' => 'mysqli://user:pass@localhost/civicrm',
  );
3)Navigate to admin >> logs >> status to see if the module is reporting any problems.
4)Edit a contact and change the postal code to enqueue the contact for lookup.  The Congressional 
  District will be retrieved the next time cron is run.


* BATCH PROCESSING *
You can batch process all contacts in CiviCRM (while reviewing them for any format issues with the
existing stored values) by running update.php and manually choosing update #2.
TODO: create an admin tab for this and use Batch API.


* USING THE API *
Here are the functions.  See the docbook comments for more info.
cd_sunlight_contact_enqueue($contact_id) 
cd_sunlight_contact_dequeue($contact_id) 
cd_sunlight_contact_get_cd($contact_id) 
cd_sunlight_contact_get_state($contact_id) 
cd_sunlight_contact_get_numnber($contact_id)
cd_sunlight_user_get_cd($uid) 
cd_sunlight_user_get_state($contact_id) 
cd_sunlight_user_get_number($contact_id) 
cd_sunlight_cd_load($cd, $return_properties = array('legislators')) 
cd_sunlight_cd_all() 
cd_sunlight_legislators_get($params = array(), $order_by = array('state' => 'ASC', 'number' => 'ASC', 'lastname' => 'ASC'))
cd_sunlight_validate_civicrm_data($contact) 
cd_sunlight_geocode_contacts($number_of_contacts = 300) 


* DEVELOPED BY *
Advomatic LLC
http://advomatic.com


* SPONSORED BY *
Democrats.com
http://democrats.com

