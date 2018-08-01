<?php 
require_once __DIR__."/db/db_config.php";
require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php";

/*
 * @author (scVnner)
 */

class Search extends getDatabase{
	
	protected const PREFIXES = array("user::", "post::", "topic::");	
	protected const REDIR = array(
							"/show/posts?tags=",
							"/show/users",
							"/forum/thread/"
							);
	protected $Input,
		   	  $Item, 
			  $item_info;	

	public function __construct(){
		$this->db_conf();
		$this->ProcessSearch();
	}

	public function ProcessSearch(){
		$this->Input = $_GET['search'];
		if(strpos($this->Input, "post::") !== 0 && strpos($this->Input, "user::") !== 0 && strpos($this->Input, "topic::") !== 0){
			$this->Item = $this->Input;	
			User::redirect(self::REDIR[0].$this->Item);
		}
		else{
			$this->item_info = explode("::", $this->Input);
		}
		
		if($this->item_info){
			if($this->item_info[0] == "post"){
				for($i = 1 ; $i < sizeof($this->item_info); $i++){// in case there's 2x:: or more
					$this->Item .= $this->item_info[$i];
				}
				User::redirect(self::REDIR[0].$this->Item);
			}

			else if($this->item_info[0] == "user"){
				for($l = 1 ; $l < sizeof($this->item_info) ; $l++){// in case there's 2x:: or more
					$this->Item .= $this->item_info[$l];
				}
				User::redirect(self::REDIR[1]."#".$this->Item);
			}
		}	
	}
}

$search = new Search();

?>
