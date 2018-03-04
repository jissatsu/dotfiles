<?php 

require_once $_SERVER['DOCUMENT_ROOT']."/assets/db_conf1FXa4.php";

/**
*
*/
class UserRegisterChecker extends getDatabase{
	
	protected $full_name, $user_name, $pass1, $pass2, $passToInsert, $email, $day, $month, $year;
	protected const METHOD = 'POST';

	public function check_register(){
		if($_SERVER['REQUEST_METHOD'] == self::METHOD){
			if(isset($_POST['buttz'])){
				if($_POST['full-name'] !== ""){
					$field1 = strval($_POST['full-name']);	
					if(preg_match("/([\$\#\@\~\!\%\^\&\*\(\)\_\+\=\-\.\,\?\<\>\'\"\:\;\[\]\{\}\`\/0-9])/", $field1)){
						echo "<span id='field1_err'>Поправете това поле</span>";
					} else{
						if(!preg_match("/([A-Z]+[a-z]+\\s+[A-Z]+[a-z])/", $field1)){
							echo "<span id='field1_err'>Поправете това поле</span>";
						} else{
							$this->full_name = $this->db->real_escape_string($field1);
						}
					}
				} else{
					echo "<span id='field1_err'>Попълнете това поле</span>";
				}

				if($_POST['u-name'] !== ""){
					$field2 = strval($_POST['u-name']);
					if(preg_match("/([\'\"])/", $field2)){
						echo "<span id='field2_err'>Поправете това поле</span>";
					} else{
						$this->user_name = $this->db->real_escape_string($field2);
					}
				} else{
					echo "<span id='field2_err'>Попълнете това поле</span>";
				}

				if($_POST['pass1'] !== ""){
					$field3 = strval($_POST['pass1']);	
					if(preg_match("/([\'\"])/", $field3)){
						echo "<span id='field3_err'>Поправете това поле</span>";
					} else{
						if(!preg_match("/([0-9]{3})/", $field3)){
							echo "<span id='field3_err'>Добавете поне 3 цифри</span>";
						} else{
							$pass1Len = strlen($field3);
							if($pass1Len < 11){
								echo "<span id='field3_err'>Твърде кратка парола</span>";
							} else{
								$this->pass1 = $this->db->real_escape_string($field3);
							}
						}
					}
				} else{
					echo "<span id='field3_err'>Попълнете това поле</span>";
				}

				if($_POST['pass2'] !== ""){
					$this->pass2 = $this->db->real_escape_string($_POST['pass2']);	
				} else{
					echo "<span id='field4_err'>Попълнете това поле</span>";
				}
				
				if($_POST['e-mail'] !== ""){
					$this->email = $this->db->real_escape_string($_POST['e-mail']);
				} else{
					echo "<span id='field5_err'>Попълнете това поле</span>";
				}

				if($_POST['dayZ'] !== ""){
					$field6 = strval($_POST['dayZ']);
					if(preg_match("/([\$\#\@\~\!\%\^\&\*\(\)\_\+\=\-\.\,\?\<\>\'\"\:\;\[\]\{\}\`\/A-Za-z])/", $field6)){
						echo "<span id='field6_err'>*</span>";
					}else{
						if(intval($field6) <= 0 || intval($field6) > 31){
							echo "<span id='field6_err'>*</span>";
						} else{
							$this->day = intval($field6);
						}
					}
				} else{
					echo "<span id='field6_err'>*</span>";
				}
				
				if($_POST['yearZ'] !== ""){
					$field8 = strval($_POST['yearZ']);
					if(preg_match("/([\$\#\@\~\!\%\^\&\*\(\)\_\+\=\-\.\,\?\<\>\'\"\:\;\[\]\{\}\`\/A-Za-z])/", $field8)){
						echo "<span id='field8_err'>*</span>";
					} else{
						if(intval($field8) < 1960 || intval($field8) > date('Y')){
							echo "<span id='field8_err'>*</span>";
						} else{
							$this->year = intval($field8);
						}
					}
				} else{
					echo "<span id='field8_err'>*</span>";
				}

				if(intval($_POST['select0h']) !== 0){
					$this->month = intval($_POST['select0h']);					
				} else{
					echo "<span id='field7_err'>*</span>";
				}
			}
		}	
		
		if($this->full_name && $this->user_name && $this->pass1 && $this->pass2 && $this->email && $this->day && $this->month && $this->year ){
			$this->check_passwords_equality($this->pass1, $this->pass2);
		}
	}

	public function check_passwords_equality($x, $y){
		if($x !== $y || $y !== $x ){
			echo "<span id='field3_err'>Паролите не съвпадат!</span>";
			return false;
		} else{
			$this->passToInsert = $this->db->real_escape_string($y);
			$this->passToInsert = password_hash($this->passToInsert, PASSWORD_DEFAULT);

			$this->check_duplicate_credentials($this->user_name, $this->email);
		}
	}

	public function check_duplicate_credentials($uname, $mail){
		$arr = array();
		$getCredentials = "SELECT * FROM membaz";
		$go = $this->db->query($getCredentials);
		while($result = $go->fetch_assoc()){
			foreach($result as $key => $value){
				array_push($arr, $value);
			}  
		}
		if(in_array($uname, $arr) || in_array($mail, $arr)){
			echo "<span id='top_err'>Потребителско име или имейл вече регистрирани</span>";
		} else{
			$this->insert_data($this->full_name, $this->user_name, $this->email, $this->passToInsert);
		}
	}

	public function insert_data($fn, $un, $em, $pss){
		$getLastId = "SELECT MAX(id) AS idd FROM membaz";
		$insertQuery = "INSERT INTO membaz (uName, uUser, uMail, uPass, uSignDate, uBirth) VALUES (?, ?, ?, ?, ?, ?)";
		$signupDate = '['.date('Y').'-'.date('m').'-'.date('d').']';
		$birth = intval($this->day) . '/' . intval($this->month) . '/' . intval($this->year);
		$smart = $this->db->prepare($insertQuery);
		$smart->bind_param('ssssss', $fn, $un, $em, $pss, $signupDate, $birth);
		$smart->execute();
	}
}


?>
