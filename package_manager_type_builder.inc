<?php
/**
* +--------------------------------------------------------------------+
* | This MySource Matrix Module file is Copyright (c) Squiz Pty Ltd    |
* | ACN 084 670 600                                                    |
* +--------------------------------------------------------------------+
* | IMPORTANT: This Module is not available under an open source       |
* | license and consequently distribution of this and any other files  |
* | that comprise this Module is prohibited. You may only use this     |
* | Module if you have the written consent of Squiz.                   |
* +--------------------------------------------------------------------+
*
* $Id$
*
*/

require_once SQ_INCLUDE_PATH.'/package_manager.inc';

/**
* Search_Manager
*
* Purpose
*    Manages the install and upgrade of the Search package and Search assets,
*    uses info gleened from the package.xml and asset.xml files
*
* @author  Blair Robertson <brobertson@squiz.net>
* @version $Revision$
* @package MySource_Matrix_Packages
* @subpackage search
*/
class Package_Manager_Type_builder extends Package_Manager
{

	protected $_assetDirectory;

	/**
	* Constructor
	*
	*/
	function __construct()
	{
		$this->_assetDirectory = SQ_DATA_PATH . '/custom_assets';
		$this->_createCustomAssetsFolder();

		$this->_full_path = SQ_PACKAGES_PATH . '/type_builder';
		parent::__construct();

	}//end constructor


	/**
	* Recursivly searches through the passed package's directory for asset.xml files
	* and enter's them into the $this->assets array with the details
	*
	* @access private
	*/
	function _loadPackageAssets()
	{
		$this->assets = Array();

		$this->_recurseLoadPackageAssets($this->assets, $this->_full_path, $this->_base_path);
		// we need to be looking in the data directory for our assets
		$this->_recurseLoadPackageAssets($this->assets, $this->_assetDirectory, substr($this->_assetDirectory, strlen(SQ_SYSTEM_ROOT) + 1));

		// reload the asset types here :/
		// otherwise any changes in asset locations will not be picked up by the asset manager
		$GLOBALS['SQ_SYSTEM']->am->_loadAssetTypes();

	}//end _loadPackageAssets()



	/**
	* Returns an array of all the asset types in the package that are to be treated as system assets
	* NOTE: Assets will be installed in the order they appear in this list
	*
	* @return array
	* @access public
	* @static
	*/
	function getSystemAssetTypes()
	{
		return Array('asset_type_builder');

	}//end getSystemAssetTypes()


	/**
	* Create the search manager system asset
	*
	* @return object
	* @access public
	*/
	function &createAssetTypeBuilder()
	{
		$system_management_folder = $GLOBALS['SQ_SYSTEM']->am->getSystemAsset('system_management_folder');

		$GLOBALS['SQ_SYSTEM']->am->includeAsset('asset_type_builder');
		$system_asset = new asset_type_builder();
		$system_asset_link = Array('asset' => &$system_management_folder, 'link_type' => SQ_LINK_TYPE_1, 'is_exclusive' => 1);
		if (!$system_asset->create($system_asset_link)) {
			trigger_localised_error('SCH0007', E_USER_ERROR);
		}
		pre_echo('Asset Type Builder Asset Id : '.$system_asset->id);

		$GLOBALS['SQ_SYSTEM']->am->registerSystemAsset('asset_type_builder', $system_asset->id);

		return $system_asset;

	}//end createAssetTypeBuilder()
	
	
	/**
	* Purpose
	*
	* @param type	$p	desc
	*
	* @return bool
	* @access protected
	*/
	protected function _createCustomAssetsFolder()
	{
		$ret = FALSE;
		
		$path = $this->_assetDirectory;
		if (!file_exists($path)) {
			$rets = @mkdir($path);
			//pre_echo('Create folder '.$path.': '.($rets ? 'OK' : 'FAILED'));
		}
	
		if (file_exists($path)) {
			$rets = @system('chmod -R ug+w '.escapeshellarg($path).' > /dev/null 2> /dev/null ');
			//pre_echo('Set permissions for '.$path.': '.(($rets !== FALSE) ? 'OK' : 'FAILED'));
		}

		return $ret;
	
	}//end 
	

}//end class

