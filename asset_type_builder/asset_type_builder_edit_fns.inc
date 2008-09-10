<?php /**
* +--------------------------------------------------------------------+
* | Squiz.net Open Source Licence                                      |
* +--------------------------------------------------------------------+
* | Copyright (c), 2003 Squiz Pty Ltd (ABN 77 084 670 600).            |
* +--------------------------------------------------------------------+
* | This source file may be used subject to, and only in accordance    |
* | with, the Squiz Open Source Licence Agreement found at             |
* | http://www.squiz.net/licence.                                      |
* | Make sure you have read and accept the terms of that licence,      |
* | including its limitations of liability and disclaimers, before     |
* | using this software in any way. Your use of this software is       |
* | deemed to constitute agreement to be bound by that licence. If you |
* | modify, adapt or enhance this software, you agree to assign your   |
* | intellectual property rights in the modification, adaptation and   |
* | enhancement to Squiz Pty Ltd for use and distribution under that   |
* | licence.                                                           |
* +--------------------------------------------------------------------+
*
* $Id$
*
*/


require_once SQ_INCLUDE_PATH.'/asset_edit/asset_edit_fns.inc';

/**
* Asset_Type_Builder_Edit_Fns
*
* Purpose
*
*
* @author  Geoffroy Noel <gnoel@squiz.co.uk>
* @version $Revision$
* @package Squiz_UK_Clients
* @subpackage type_builder
*/
class Asset_Type_Builder_Edit_Fns extends Asset_Edit_Fns
{
	
	var $_build_error = '';

	/**
	* Constructor
	*
	*/
	function __construct()
	{
		parent::__construct();

	}//end constructor


	/**
	* Paint the interface for regenerating the report
	*
	* @param Report				$asset	the asset whose interface we are painting
	* @param Backend_Outputter	$o		the backend outputter class
	* @param string				$prefix	prefix for the form element
	*
	* @return boolean
	* @access public
	*/
	public function paintBuildAssetType($asset, Backend_Outputter $o, $prefix)
	{
		if ($asset->writeAccess('attributes')) {
			$parent_types = Array();
			$asset_types = $GLOBALS['SQ_SYSTEM']->am->_asset_types;
			foreach ($asset_types as $type_code => $info) {
				$parent_types[$type_code] = $info['name'];
			}
			asort($parent_types);
			
			$type_post = array_get_index($_POST, $prefix.'_type_info', Array('name' => '', 
										'parent_type' => 'page_standard', 'description' => '', 'flash_menu_path' => '',
										'hide_from_menu' => 0));

			echo '<table>';
			echo '<tr><td>';
			echo 'Inherit From*</td><td>';
			combo_box($prefix.'_type_info[parent_type]', $parent_types, FALSE, $type_post['parent_type'], 0);
			echo '</td></tr><tr><td>';
			echo 'Display Name*</td><td>';
			text_box($prefix.'_type_info[name]', $type_post['name'], 30);
			echo '<br/>Please avoid any special character. Only alphanumeric and spaces are allowed. You can capitalise words.';
			echo '</td></tr><tr><td>';
			echo 'Menu</td><td>';
			text_box($prefix.'_type_info[flash_menu_path]', $type_post['flash_menu_path'], 30);
			echo '&nbsp;';
			check_box($prefix.'_type_info[hide_from_menu]', '1', !empty($type_post['hide_from_menu']), 30);
			echo ' Hide from the menu?';
			echo '<br/>If empty, the menu path will be "Custom Assets"';
			echo '</td></tr><tr><td>';
			echo 'Description</td><td>';
			text_box($prefix.'_type_info[description]', $type_post['description'], 100);
			echo '<br/>If empty, the description will be the name of the asset';
			echo '</td></tr></table>';
			
			$type_create = array_get_index($_POST, $prefix.'_type_info', Array());
			if (!empty($type_create['name'])) {
				$type_create['instantiable'] = empty($type_create['hide_from_menu']) ? 1 : 0;
				$this->_build_error = $asset->createAssetType($type_create);
				if (!empty($this->_build_error)) {
					echo '<span style="color:red;font-weight:bolder;">'.$this->_build_error.'</span></br>';
				} else {
					echo '<span style="color:green;font-weight:bolder;">'.'Creation successful!'.'</span></br>';
				}
			}
			
			//check_box($prefix.'_regenerate');
			//label(translate('core_generate_report'), $prefix.'_regenerate');
			return TRUE;
		}
		return FALSE;

	}//end paintBuildAssetType()


	/**
	* Processes the interface for regenerating the report
	*
	* @param Report				$asset	the asset whose interface we are painting
	* @param Backend_Outputter	$o		the backend outputter class
	* @param string				$prefix	prefix for the form element
	*
	* @return boolean
	* @access public
	*/
	public function processBuildAssetType($asset, Backend_Outputter $o, $prefix)
	{
		if (empty($_POST[$prefix.'_type_info'])) {
			return FALSE;
		}
		
		return TRUE;

	}//end processBuildAssetType()


	/**
	* Paint the interface for regenerating the report
	*
	* @param Report				$asset	the asset whose interface we are painting
	* @param Backend_Outputter	$o		the backend outputter class
	* @param string				$prefix	prefix for the form element
	*
	* @return boolean
	* @access public
	*/
	public function paintRemoveAssetType($asset, Backend_Outputter $o, $prefix)
	{
		if ($asset->writeAccess('attributes')) {
			
			$asset_types = $asset->getAllCustomTypes();
			$remove_types = Array('_' => '-- Select Type --');
			foreach ($asset_types as $type_code => $info) {
				$remove_types[$type_code] = $info['name'];
			}
			asort($remove_types);
			
			echo '<table>';
			echo '<tr>';
			echo '<td>Delete Type</td><td>';
			combo_box($prefix.'_remove_type_code', $remove_types, FALSE, '_', 0);
			echo '</td></tr></table>';
			
			$remove_type_code = array_get_index($_POST, $prefix.'_remove_type_code', '_');
			if ($remove_type_code !== '_') {
				$retr = $asset->removeAssetType($remove_type_code);
				if (!empty($retr)) {
					echo '<span style="color:red;font-weight:bolder;">'.$retr.'</span></br>';
				} else {
					echo '<span style="color:green;font-weight:bolder;">'.'Deletion successful!'.'</span></br>';
				}
			}
			
			return TRUE;
		}
		return FALSE;

	}//end paintBuildAssetType()


	/**
	* Processes the interface for regenerating the report
	*
	* @param Report				$asset	the asset whose interface we are painting
	* @param Backend_Outputter	$o		the backend outputter class
	* @param string				$prefix	prefix for the form element
	*
	* @return boolean
	* @access public
	*/
	public function processRemoveAssetType($asset, Backend_Outputter $o, $prefix)
	{
		if (empty($_POST[$prefix.'_type_info'])) {
			return FALSE;
		}
		
		return TRUE;

	}//end processBuildAssetType()


}//end class
?>