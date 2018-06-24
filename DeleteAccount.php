<?php 
@session_start();
require_once __DIR__."/db/db_config.php";
require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php";

/*
 * @author Antonio Drandarov(scVnner)
 */

class RemoveAccount extends getDatabase{
	
	public $uName, $uID;

	public function __construct(){
		$data = json_decode($_POST['data']);
		$this->uName = User::sanitizeInput($data->uName);
		$this->uID = (int) $data->uId;
		$this->userFileName = dirname(__FILE__)."/users/".$this->uID.".php";

		$this->queries = array(0 => "SELECT postName FROM posts WHERE fromUserID=?",
							   1 => "SELECT p_image FROM uzer WHERE id=?",
							   2 => "DELETE FROM uzer WHERE id=?",
						   	   3 => "DELETE FROM posts WHERE fromUserID=?",
							   4 => "DELETE FROM followings WHERE followerID=? OR followedID=?",
							   5 => "DELETE FROM comments WHERE fromUserID=?",
							   6 => "DELETE FROM favourites WHERE fromUserID=?");
	}

	public function ProceedAccountRemoval(){	
		$getImage = $this->conn->prepare($this->queries[1]);
		$getImage->bind_param('i', $this->uID);
		$getImage->execute();

		$rresp = @$getImage->get_result();
		$ddata = @$rresp->fetch_assoc();
		
		if(!is_null($ddata['p_image'])){
			unlink($_SERVER['DOCUMENT_ROOT']."/rei-assets/avatars/".$ddata['p_image']);
		}

		$getPosts = $this->conn->prepare($this->queries[0]);
		$getPosts->bind_param('i', $this->uID);
		$getPosts->execute();

		$resp = @$getPosts->get_result();
		while($data = @$resp->fetch_assoc()){
			foreach($data as $postName){
				unlink(dirname(__FILE__)."/posts/".$postName);
			}
		}

		for($i = 2 ; $i < sizeof($this->queries) ; $i++){
			if($i == 4){
				$accountRemoval = $this->conn->prepare($this->queries[$i]);
				$accountRemoval->bind_param('ii', $this->uID, $this->uID);
				$accountRemoval->execute();
			} else {
				$accountRemoval = $this->conn->prepare($this->queries[$i]);
				$accountRemoval->bind_param('i', $this->uID);
				$accountRemoval->execute();
			}
		}

		unset($_SESSION['uzer']);

		if($this->userFileName){
			unlink($this->userFileName);
		}
	}
}

$RemoveAccount = new RemoveAccount();
$RemoveAccount->db_conf();
$RemoveAccount->ProceedAccountRemoval();

?>
