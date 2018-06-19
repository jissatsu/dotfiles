<?php 
require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php"; 
require_once __DIR__."/db/db_config.php"; 

/*
 * @author Antonio Drandarov(scVnner)
 */

class RecoveryRequest extends getDatabase{
	
	public $gotEmail;

	public function getUserData(){
		if(User::requestMethod("post")){
			$email = User::sanitizeInput($_POST['email']);
			if($email == ""){
				echo "<span id='mail-err-1'>Полето остана празно!</span>";
			} else {
				$this->gotEmail = $email;
				$this->checkEmailExists($this->gotEmail);
			}	
		}	
	}

	public function checkEmailExists( $mail ){
		$checkEmail = "SELECT id FROM uzer WHERE email=?";
		$check = $this->conn->prepare($checkEmail);		
		$check->bind_param('s', $mail);
		$check->execute();

		$resp = @$check->get_result();
		$data = @$resp->fetch_assoc();

		if(is_null($data['id'])){
			echo "<span id='mail-err-2'>Няма регистриран такъв имейл!</span>";
		} else {
			$id = (int) $data['id'];
			self::sendEmail( $id, $mail );
		}
	}

	public static function sendEmail( $id, $mail ){
		$authCode = uniqid().rand(1, 1000).uniqid();
		$message = "http://www.cyber-chann.bg/user/recover-password?id=".$id."&authCode=".$authCode;
		$send = shell_exec("cd /var/www; node mail.js " . escapeshellarg($mail) . " ". escapeshellarg($message));

		if(preg_match("/^(Email sent)$/", $send)){
			User::storeSession("authCode", "", $authCode);
			User::storeSession("authId", "", $id);
			if(preg_match("/(@abv)/", $mail)){
				User::storeSession("userEmail", "", "https://www.abv.bg/");
			} else if(preg_match("/(@gmail)/", $mail)){
				User::storeSession("userEmail", "", "https://mail.google.com");	
			}
			User::redirect("/user/password-link-request");
		}
	}

}

?>
