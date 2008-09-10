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

require_once SQ_INCLUDE_PATH.'/asset.inc';

/**
* Asset_Type_Builder
*
* Purpose
*
*
* @author  Geoffroy Noel <gnoel@squiz.co.uk>
* @version $Revision$
* @package Squiz_UK_Clients
* @subpackage type_builder
*/
class Asset_Type_Builder extends Asset
{

	/**
	* Constructor
	*
	* @param string	$assetid	the asset id to be loaded
	*
	*/
	function __construct($assetid=0)
	{
		parent::__construct($assetid);

	}//end constructor
	
	
	/**
	* Returns name of the asset
	*
	* @param boolean	$short_name	whether or not we are after the shortname or the full name
	*
	* @return string
	* @access private
	* @see Asset::_getName()
	*/
	function _getName($short_name=FALSE)
	{
		return $GLOBALS['SQ_SYSTEM']->am->getTypeInfo($this->type(), 'name');

	}//end _getName()
	
	
	/**
	* Purpose
	*
	* @param type	$p	desc
	*
	* @return bool
	* @access protected
	*/
	public function createAssetType($asset_type_info=Array())
	{
		$ret = '';

		// 1. validate and auto-complete the type info

		$asset_type_info['description'] = trim($asset_type_info['description']);
		if (empty($asset_type_info['description'])) {
			$asset_type_info['description'] = trim($asset_type_info['name']);
		}
		$asset_type_info['name'] = ucwords(trim($asset_type_info['name']));
		$asset_type_info['parent_type'] = strtolower(trim($asset_type_info['parent_type']));
		
		if (empty($GLOBALS['SQ_SYSTEM']->am->_asset_types[strtolower($asset_type_info['parent_type'])]['dir'])) {
			return "Asset type not found $parent_type";
		}
		
		$parent_type_info = $GLOBALS['SQ_SYSTEM']->am->_asset_types[strtolower($asset_type_info['parent_type'])];

		if (empty($asset_type_info['flash_menu_path'])) {
			$asset_type_info['flash_menu_path'] = 'Custom Assets';
		}
		if (empty($asset_type_info['package'])) {
			$asset_type_info['package'] = 'type_builder';
		}
		if (empty($asset_type_info['type_code'])) {
			$asset_type_info['type_code'] = strtolower(str_replace(' ' ,'_', $asset_type_info['name']));
		}
		
		// 1.2 get parent type
		if (empty($asset_type_info['instantiable'])) {
			$asset_type_info['instantiable'] = $parent_type_info['instantiable'] ? 'yes' : 'no';
		}
		if (empty($asset_type_info['allowed_access'])) {
			$asset_type_info['allowed_access'] = $parent_type_info['allowed_access'];
		}

		if (empty($asset_type_info['sub_path'])) {
			$asset_type_info['sub_path'] = '';
		} else {
			$asset_type_info['sub_path'] = rtrim($asset_type_info['sub_path'], '/').'/'; 
		}
		
		$asset_type_info['path'] = SQ_PACKAGES_PATH.'/'.$asset_type_info['package'].'/custom_assets/'.$asset_type_info['sub_path'].$asset_type_info['type_code'];
		
		$asset_type_info['header'] = $this->_getHeaders();
		
		// 2. remove and recreate existing asset folder
		$ret_dir = $this->_recreateFolder($asset_type_info['path']);
		if (!empty($ret_dir)) {
			return $ret_dir;
		}
		
		
		// 3. creation
		$asset_type_info['requirement'] = '';
		if ($asset_type_info['parent_type'] !== 'asset') {
			$asset_type_info['requirement'] = '<requirement type="asset">
			<code>'.$asset_type_info['parent_type'].'</code>
			<version>0.1</version>
		</requirement>';
		}
		
		$asset_type_info['parent_class_name'] = $asset_type_info['parent_type'];

		$asset_type_info['class_name'] = str_replace(' ', '_', $asset_type_info['name']);
		
		$asset_type_info['require_once_edit_fns'] = $this->_getRequirePath($asset_type_info['parent_type'], 'edit_fns');
		$asset_type_info['require_once'] = $this->_getRequirePath($asset_type_info['parent_type'], '');
		
		if ($handle = opendir(dirname(__FILE__).'/templates')) {
		    while (false !== ($file = readdir($handle))) {
		    	if (preg_match('/^(.*).template$/', $file, $match)) {
		    		$file_dest = str_replace('__asset__', $asset_type_info['type_code'], $match[1]);
		    		$contents = file_get_contents(dirname(__FILE__).'/templates/'.$file);
		    		
		    		foreach($asset_type_info as $key => $val) {
		    			$contents = str_replace('%'.$key.'%', $val, $contents);
		    		}
		    		
		    		$file_dest_full = $asset_type_info['path'].'/'.$file_dest;
		    		if (FALSE === file_put_contents($file_dest_full, $contents)) {
						return 'Cannot write file ('.$file_dest_full.')';
		    		} else {
		    			// sets the permissions and group
		    			chmod($file_dest_full, 0775);
		    		}
		    	}
		    }
		}
		
		return $ret;
	
	}//end _createAssetType()
	
	
	/**
	* Purpose
	*
	* @param type	$p	desc
	*
	* @return bool
	* @access protected
	*/
	protected function _getHeaders()
	{
		$ret = '/**
* +- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
* | This MySource Matrix CMS file is Copyright (c) Squiz Pty Ltd       |
* | ACN 084 670 600                                                    |
* +- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
* | IMPORTANT: Your use of this Software is subject to the terms of    |
* | the Licence provided in the file licence.txt. If you cannot find   |
* | this file please contact Squiz (www.squiz.net) so we may provide   |
* | you a copy.                                                        |
* +- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
*
* $Id$
*
*/';
	
		return $ret;
	
	}//end
	
	
	/**
	* Purpose
	*
	* @param type	$p	desc
	*
	* @return bool
	* @access protected
	*/
	protected function _getClassName($type_code)
	{
		$ret = str_replace(' ', '_', ucwords(str_replace('_', ' ', $type_code)));
	
		return $ret;
	
	}//end	 
	
	
	/**
	* Purpose
	*
	* @param type	$p	desc
	*
	* @return bool
	* @access protected
	*/
	protected function _getRequirePath($parent_type, $type='edit_fns')
	{
		if (strtolower($parent_type) == 'asset') {
			if ($type == '') {
				$parent_path = "SQ_INCLUDE_PATH.'/asset.inc'";
			} else {
				$parent_path = "SQ_INCLUDE_PATH.'/asset_edit/asset_edit_fns.inc'";
			}
		} else {
			$parent_path = $GLOBALS['SQ_SYSTEM']->am->_asset_types[strtolower($parent_type)]['dir'];
			if ($type == '') {
				$parent_path .= "/$parent_type.inc";
			} else {
				$parent_path .= "/$parent_type"."_edit_fns.inc";
			}
			$parent_path = strtolower($parent_path);
	
			$parent_path = preg_replace('/packages/', 'SQ_PACKAGES_PATH.\'', $parent_path);
			$parent_path = preg_replace('/core\/assets/', 'SQ_CORE_PACKAGE_PATH.\'', $parent_path);
	
			$parent_path .= '\'';
		}
	
		return $parent_path;
	
	}//end 


	/**
	* Purpose
	*
	* @param type	$p	desc
	*
	* @return bool
	* @access protected
	*/
	protected function _recreateFolder($path)
	{
		if (!$this->_removeFolder($path)) {
			return 'Cannot remove existing folder ('.$path.')';
		}
		
		@mkdir($path);
		
		if (!file_exists($path)) {
			return 'Cannot create asset folder ('.$path.')';
		}
		
		return '';
	
	}//end 
	
	
	/**
	* Purpose
	*
	* @param type	$p	desc
	*
	* @return bool
	* @access protected
	*/
	protected function _removeFolder($path)
	{
		if (file_exists($path)) {
			// xxx: need to find a safer way!
			@system('rm -R '.escapeshellarg($path));
		}
		
		return !file_exists($path);
	
	}//end 
	
	
	
	/**
	* Purpose
	*
	* @param type	$p	desc
	*
	* @return bool
	* @access protected
	*/
	public function test()
	{
		$type_info = Array('name' => 'RM Article', 'description' => 'Royal Mail Article', 'parent_type' => 'page_standard');
		
		return $this->createAssetType($type_info);
	
	}//end test()
	
	
	/**
	* Purpose
	*
	* @param type	$p	desc
	*
	* @return bool
	* @access protected
	*/
	public function getAllCustomTypes()
	{
		$ret = Array();
		foreach ($GLOBALS['SQ_SYSTEM']->am->_asset_types as $type_code => $type_info) {
			if (preg_match('/type_builder\/custom_assets/i', $type_info['dir'])) {
				$ret[$type_code] = $type_info;
			}
		}
		
		return $ret;
	
	}//end
	
	
	/**
	* Purpose
	*
	* @param type	$p	desc
	*
	* @return bool
	* @access protected
	*/
	public function removeAssetType($type_code)
	{
		$ret = '';
		
		//$type_code = 'page';
		
		$descendants = $GLOBALS['SQ_SYSTEM']->am->getTypeDescendants($type_code, FALSE);
		if (!empty($descendants)) {
			return 'Cannot remove type, remove descendent types first: '.join(', ', $descendants);
		}
		
		$assetids = $GLOBALS['SQ_SYSTEM']->am->getTypeAssetids($type_code, FALSE, TRUE);
		if (!empty($assetids)) {
			return 'Cannot remove type, remove all instances first: #'.join(', #', array_keys($assetids));
		}
		
		// remove the code type
		$path = SQ_SYSTEM_ROOT.'/'.$GLOBALS['SQ_SYSTEM']->am->_asset_types[$type_code]['dir'];
		
		if (!$this->_removeFolder($path)) {
			return 'Cannot remove dir: '.$path;
		}
	
		return $ret;
	
	}//end 


}//end class


/*
 * 	

"bodycopy" => array (
    "type_code" => "bodycopy",
    "version" => "0.2",
    "name" => "Bodycopy",
    "instantiable" => "1",
    "allowed_access" => "system",
    "parent_type" => "asset",
    "dir" => "core/assets/bodycopy/bodycopy",
    "customisation" => "0",
    "description" => "BodyCopy Object, create tables to format content",
    "lvl" => "1",
    "ancestor_types" => array (
        [Empty]
    ) 
),
 
 */

?>