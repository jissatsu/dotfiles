<?php 

require_once $_SERVER['DOCUMENT_ROOT']."/R_chan/db/db_config.php";

/**
 * 
 * @author Antonio Drandarov (scVnner)
 *
 * A collection of static methods for dealing with some annoying shit
 *
 */

class User{
		
	/*
	 *  @param int $failed
	 *
	 * A static variable to store failed login attempts 
	 *
	 * and a static function to increment it 
	 *
	 */

	static public $failed = 0;

	
	static public function incrementFailed(){
		self::$failed++;
	}

	
	/*
	 * A static function to redirect user (prettified)
	 *
	 */
		
	static public function redirect( $url ){

		header("Location: ".$url);
		
	}
	
	/*
	 * A static function that returns the current script's name
	 * 
	 * @return string|int
	 */

	static public function scriptName( $delimeter ){
		$scriptName = basename($_SERVER['SCRIPT_NAME'], ".".$delimeter);
		return (is_int($scriptName)) ? (int) $scriptName : $scriptName;
	}


	/*
	 * A static function to check if an email format is valid 
	 *
	 * @return bool 
	 *
	 */	

	static public function validateEmail($email){

		return ( filter_var($email, FILTER_VALIDATE_EMAIL) ) ? true : false;
	}

	
	/*
	 *  A static function to handle user sessions 
	 *  
	 *  @return bool|string
	 *
	 */


	static public function session( $name, $item = null ){

		$session = null;

		if( is_null($item) && $name !== "" ){			
			$session = $_SESSION[$name];
			return ( $session ) ? $session : false;
	
		} else if( (!is_null($item) && $item !== "") && $name !== "" ){
			$session = $_SESSION[$name][$item];
			return ( $session ) ? $session : false;
		}
	}
	
	/*
	 * String sanitizing static function 
	 */
	
	static public function sanitizeInput( $input ){
		return preg_replace("([\'\"\`])", "", $input);
	}

	
	/*
	 * A static function to store session values 
	 * 
	 * @param string $sessName
	 * @param string $param
	 * @param string|int $paramValue
	 *
	 */

	
	static public function storeSession( $sessName = "", $param = "", $paramValue = null ){
		if($sessName !== ""){	
			if($param == ""){
				if(!is_null($paramValue)){
					if(is_int($paramValue)){
						$_SESSION["$sessName"] = (int) $paramValue;
					} else {
						$_SESSION["$sessName"] = (string) $paramValue;	
					}
				}	
			} else {
				if(!is_null($paramValue)){
					if(is_int($paramValue)){
						$_SESSION["$sessName"]["$param"] = (int) $paramValue;
					} else {
						$_SESSION["$sessName"]["$param"] = (string) $paramValue;	
					}
				}		
			}
		}
	}
	

	/*
	 * A static function for file uploading 
	 *
	 *
	 */

	
	static public function fileUploader($maxFileSize, $forbiddenExtensions = array(), $permittedExtensions = array(), $path){	
		if($_SERVER['REQUEST_METHOD'] == 'POST'){
			null;	
		}
	}

}

?>
