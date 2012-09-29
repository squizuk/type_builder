# Origin
This has been forked from https://github.com/squizuk/type_builder  
See the discussion about this in roadmap - https://rm.squiz.net/matrix/610

# Requirements
* A working install of Squiz Matrix (only tested on 4.10.2)
* PHP functions (available in Scientific Linux via the php-process RPM)
	* posix_geteuid
	* posix_getpwuid
* Knowledge of which user the webserver runs as (referred to as $WEB_USER in this document)

# Installation
1. Clone this package into your matrix packages directory
1. Run the following commands as the same user as your original matrix install (assumes you're in the matrix root directory):

		php install/step_03.php $(pwd) --package=type_builder
		php install/compile_locale.php $(pwd)
		chown -R $WEB_USER packages/type_builder
		chown -R $WEB_USER data/public/asset_types/asset_type_builder

# Usage
Open the `Asset Types` screen on the `System Management`->`Asset Type Builder` asset

## Creating types
New types require two pieces of information, namely

1. The Asset type to inherit from
1. The display name of the asset

Optional pieces of information are

1. A submenu under which to place the asset in the asset map's asset list (defaults to `Custom Assets`)
1. A description of the new asset type

To complete the installation of the new Asset type we must run `install/step_03.php` to rebuild the asset cache and put some details in the database.  This can be done manually or automatically however both methods have their pitfalls.  See [step_03](#step_03) for details.

## Deleting types
To remove a type you just need to select the appropriate type from the `Delete Type` select field and commit.  There are a few requirements to successfully removing a type:

1. No Assets of the specified type exist
1. No Asset types descend from the specified Asset type

With the above conditions met then the Asset type removal will begin.  Again, completion of the type removal requires running `install/step_03.php`.  See [step_03](#step_03) for details.

# step_03
The `install/step_03.php` script has a few requirements to complete successfully and they mostly revolve around permissions.  Assuming you have a working matrix install (ie. step_03 has already been run successfully), running step_03 to install a new type requires write permissions to the following directories:
- core/lib/DAL/Oven/core
- core/lib/DAL/Oven/*_package
- core/lib/DAL/QueryStore

If the above directories are owned by the webserver user, then the Asset Type Builder will automatically run step_03 for you and all that's left is to refresh the page.  However if you haven't changed the permissions of the above directories then it's left to you to run step_03 to complete the process.  To perform required changes you can run the following commands:

	php install/step_03.php $(pwd) --package=type_builder
	# if you're installing a type you should also...
	chown -R $WEB_USER data/public/asset_types/<new asset type>

