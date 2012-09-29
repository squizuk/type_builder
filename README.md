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

## Deleting types
To remove a type you just need to select the appropriate type from the `Delete Type` select field and commit.  There are a few requirements to successfully removing a type:

1. No Assets of the specified type exist
1. No Asset types descend from the specified Asset type


