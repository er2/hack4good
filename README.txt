$Id$

*** CIVICRM SUNLIGHT CONGRESSIONAL DISTRICTS ***


** ABOUT **

This module provides a way to automatically get congressional district information about your
contacts into CiviCRM using the Sunlight Congress API.
  http://services.sunlightlabs.com/docs/Sunlight_Congress_API/
It also fetches data about the current members of Congress.  You can access all this data via an
API or use the Views module.


** INSTALL **

1)Unpack the modules files just like any other Drupal module.  Probably at something like:
    sites/all/modules/contrib/cd_sunlight

2)Navigate to
    /admin/reports/status
  To get instructions on how to configure your settings.php to use the module.  Specifically you
  must have prefixes for several CiviCRM tables in settings.php.  

3)Configure the module at
    /admin/settings/cd_sunlight
  Specifically the Sunlight API key.

4)Link with CiviCRM fields at
    /admin/settings/cd_sunlight/fields
  Follow the links to create new fields in CiviCRM.

5)Return to the status report to see if the module is reporting any problems
    /admin/reports/status

6)If you wish you can enqueue all CiviCRM contacts to have their congressional district retrieved
  during the next cron run at
    /admin/settings/cd_sunlight/batch

7)Edit a contact and change the postal code to enqueue the contact for lookup.  The Congressional
  District will be retrieved the next time cron is run.  When a contact is edited from a CiviCRM 
  profile (ex. /user/12345/edit/[civicrm profile name]) the congressional district will be 
  immediately retrieved.



** VIEWS INTEGRATION **

To show information about members of Congress on your site you need to install the Views module.
  http://drupal.org/project/views
You can find the default view called "Congress" at
  /admin/build/views
You can either customize it or create a new view of the type "Congress".


** USING THE API **

The API should only be used in situations where Views cannot be used.  In the future the Views 
integration will likely see more tending than the API.  Here are the API functions.  See the
docbook comments for more info.

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
cd_sunlight_cd_get_all()
cd_sunlight_cd_get_all_used()
cd_sunlight_legislators_get($params = array(), $order_by = array('state' => 'ASC', 'district' => 'ASC', 'lastname' => 'ASC'))
cd_sunlight_validate_civicrm_data($contact)
cd_sunlight_cd_parse_state($cd)
cd_sunlight_cd_parse_district($cd)
cd_sunlight_state_abbr2name($state_abbr);
cd_sunlight_state_name2abbr($state_name);


** CONGRESS MODULE **

DEPRICATED.  The congress module is an example implementation of the API.  New users should use
Views instead.  It will be removed in cd_sunlight 3.x or Drupal 7 (whichever comes first) existing
users should replace with Views before then.


** DEVELOPED BY **

Advomatic LLC
http://advomatic.com


** SPONSORED BY **

Democrats.com
http://democrats.com
