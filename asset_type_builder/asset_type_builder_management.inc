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

require_once SQ_INCLUDE_PATH.'/asset_management.inc';

/**
* Asset_Type_Builder_Management
*
* @see Asset_Management
*
* @author  Geoffroy Noel <gnoel@squiz.co.uk>
* @version $Revision$
* @package Squiz_UK_Clients
* @subpackage type_builder
*/
class Asset_Type_Builder_Management extends Asset_Management
{

	/**
	* Constructor
	*
	*/
	function Asset_Type_Builder_Management(&$pm)
	{
		$this->Asset_Management($pm);
		$this->vars = Array(
							/*
							'contents' => Array(
								'added'		=>	'0.0.1',
								'type'		=>	'text',
								'default'	=>	'',
							),
							'weblink' => Array(
								'added'		=>	'0.0.1',
								'type'		=>	'text',
								'default'	=>	'',
							),
							'title' => Array(
								'added'		=>	'0.0.1',
								'type'		=>	'text',
								'default'	=>	'',
							),
							*/
						);

	}//end constructor


}//end class
?>