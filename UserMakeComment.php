<?php 
@session_start();
require_once __DIR__."/db/db_config.php";
require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php";

/*
 * @author Antonio Drandarov (scVnner)
 */

class MakeComment extends getDatabase{
	
	private $data;

	public function __construct(){
		$this->db_conf();
		$this->data = json_decode($_POST['data']);
		$this->id = (int) User::session("uzer", "id");						  					   #your id
		$this->name = User::sanitizeInput(User::session("uzer", "log_name")); 					   #your username
		$this->comment = $this->data->com;									  					   #comment value
		$this->date = preg_replace("/([^0-9\/\-\:])/", "", User::sanitizeInput($this->data->d));   #comment date
		$this->postID = (int) User::sanitizeInput($this->data->p);			  					   #the post id

		if($this->id && $this->name){
			if($this->comment == ""){
				echo "c=Полето остана празно!,";
			} else {
				if(strlen($this->comment) > 185){
					echo "c=Твърде много символи! [max=185],";
				} else {
					$this->comment = preg_replace("/([^[:print:]])/", "", $this->comment); #removing all non-printable characters
					$this->comment = preg_replace("/([\'])/", "{xs|sing}", $this->comment);
					$this->comment = preg_replace("/([\"])/", "{xd|doub}", $this->comment);
					$this->comment = preg_replace("/([\`])/", "{xd|strange}", $this->comment);
					$this->comment = preg_replace("/([\<])/", "{xl|left}", $this->comment);
					$this->comment = preg_replace("/([\>])/", "{xr|right}", $this->comment);
					$this->ProcessUserComment($this->comment);
				}
			}
		}
	}

	public function ProcessUserComment( $comVal ){
		$query = "INSERT INTO comments (commentDate, fromUserName, onPost, fromUserID, comment) VALUES (?, ?, ?, ?, ?)";				
		$comment = $this->conn->prepare($query);	
		$comment->bind_param('ssiis', $this->date, $this->name, $this->postID, $this->id, $comVal);
		if($comment->execute()){
			echo "n!";
		}
	}
}

$MakeComment = new MakeComment();
?>
