<?php 

@session_start();
require_once __DIR__."/db/db_config.php";
require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php";


/*
 * @author Antonio Drandarov (scVnner)
 *
 */


class updateUserInfo extends getDatabase{

	protected $id, $name, $birth, $user, $git, $twitter, $facebook, $skype, $instagram;

	public function getData(){
		$data = json_decode($_POST['data']);
		
		$this->id = intval($data->id);
		$this->name = User::sanitizeInput($data->name);
		$this->birth = User::sanitizeInput($data->birth);
		$this->user = User::sanitizeInput($data->user);
		
		$this->github = User::sanitizeInput($data->github);
		$this->twitter = User::sanitizeInput($data->twitter);
		$this->facebook = User::sanitizeInput($data->facebook);
		$this->skype = User::sanitizeInput($data->skype);
		$this->instagram = User::sanitizeInput($data->instagram);

		$this->name = ($this->name == "Няма") ? null : $this->name;
		$this->birth = ($this->birth == "Няма") ? null : $this->birth;
	
		if($this->user == ""){
			$this->user = "user=*,";
			echo $this->user;
		} else {
			$this->user = $this->user;
		}
		

		if($this->github !== "@github"){
			if(substr($this->github, 0, 1) !== "@" || $this->github == "@" || substr_count($this->github, "@") > 1){
				$github = "github=*,";
				echo $github;
			} else {
				$github = $this->github;
			}
		} else {
			$github = null;
		}

		if($this->twitter !== "@twitter"){
			if(substr($this->twitter, 0, 1) !== "@" || $this->github == "@" || substr_count($this->twitter, "@") > 1){
				$twitter = "twitter=*,";
				echo $twitter;
			} else {
				$twitter = $this->twitter;
			}
		} else {
			$twitter = null;
		}

		if($this->facebook !== "@facebook"){
			if(substr($this->facebook, 0, 1) !== "@" || $this->facebook == "@" || substr_count($this->facebook, "@") > 1){
				$facebook = "facebook=*,";
				echo $facebook;
			} else {
				$facebook = $this->facebook;
			}
		} else {
			$facebook = null;
		}

		if($this->skype !== "@skype"){
			if(substr($this->skype, 0, 1) !== "@" || $this->skype == "@" || substr_count($this->skype, "@") > 1){
				$skype = "skype=*,";
				echo $skype;
			} else {
				$skype = $this->skype;
			}
		} else {
			$skype = null;
		}

		if($this->instagram !== "@instagram"){
			if(substr($this->instagram, 0, 1) !== "@" || $this->instagram == "@" || substr_count($this->instagram, "@") > 1){
				$instagram = "instagram=*";
				echo $instagram;
			} else {
				$instagram = $this->instagram;
			}
		} else {
			$instagram = null;
		}
		

		if($this->user !== "user=*," && $github !== "github=*," && $twitter !== "twitter=*," && $facebook !== "facebook=*," && $skype !== "skype=*," && $instagram !== "instagram=*"){
			$this->processQueryData($this->name, $this->birth, $this->user, $github, $twitter, $facebook, $skype, $instagram, $this->id);	
		}
	}


	public function processQueryData($name, $birth, $user, $git, $twitter, $facebook, $skype, $instagram, $id){	
		$qqq = "UPDATE uzer SET name=?, birth=?, username=?, github=?, twitter=?, facebook=?, skype=?, instagram=? WHERE id=?";
		$updateName = "";
		User::storeSession("uzer", "log_name", $user);
		@session_regenerate_id();
		$update = $this->conn->prepare($qqq);
		$update->bind_param('ssssssssi', $name, $birth, $user, $git, $twitter, $facebook, $skype, $instagram, $id);
		if($update->execute()){
			
			$file = $_SERVER['DOCUMENT_ROOT']."/R_chan/users.xml";

			$doc = new DOMDocument();
			$xml = file_get_contents($file);
											
			$doc->loadXML($xml);
											
			$root = $doc->firstChild;
											
			$user = $root->getElementsByTagName('user');

			foreach($user as $userNode){
				if(intval($userNode->getAttribute('id')) == intval($this->id)){
					if($userName = $userNode->getElementsByTagName('username')->item(0)){
						$userNode->removeChild($userName);			
					}
													
					$userName = $doc->createElement('username');
					$userName->appendChild($doc->createTextNode($this->user));
					$userNode->insertBefore($userName, $userNode->getElementsByTagName('date')->item(0));
				} 
			}

			$doc->save($file);
				
			echo "done";		
		}
	}

}

$uInfo = new updateUserInfo();
$uInfo->db_conf();
$uInfo->getData();

?>
