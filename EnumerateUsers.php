<?php 
require_once __DIR__."/db/db_config.php";
require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php";

/*
 * @author (scVnner)
 */

class EnumerateUsers extends getDatabase{
	
	public function __construct(){
		$this->db_conf();	
		$this->word = "";
		$this->queries = array("get-uploaders" => 
								"SELECT * FROM (
								 SELECT posts.fromUserName, uzer.p_image, uzer.signUpDate, uzer.id, uzer.uploaded FROM posts 
								 RIGHT JOIN uzer ON posts.fromUserID = uzer.id) 
								 AS t WHERE fromUserName != 'NULL' GROUP BY fromUserName",
							   
								 "get-users" => "SELECT uzer.username, uzer.id, uzer.uploaded, uzer.p_image, uzer.signUpDate FROM uzer");

		$url_path = explode("=", parse_url($_SERVER['REQUEST_URI'], PHP_URL_QUERY));
		if($url_path[1] && $url_path[1] == "uploaders"){
			$this->param = $this->queries['get-uploaders'];
			$this->word = "ъплоудъри";
		} else{
			$this->param = $this->queries['get-users'];	
			$this->word = "потребители";
		}
	}

	public function EnumerateAll(){
		echo "<div id='uploaders-all'>Всички ".$this->word."</div>";
		echo "<section id='uploaders-inner-container'>";

		$result = $this->conn->query($this->param);
		if($result->num_rows > 0){
			while($data = $result->fetch_assoc()){
				$image = (is_null($data['p_image'])) ? htmlentities("/rei-assets/platige-image-reel-2016.jpg", ENT_QUOTES, 'UTF-8') : htmlentities($data['p_image'], ENT_QUOTES, 'UTF-8');
				$signUpDate = htmlentities($data['signUpDate'], ENT_QUOTES, 'UTF-8');
				$username = ($data['username']) ? htmlentities($data['username'], ENT_QUOTES, 'UTF-8') : htmlentities($data['fromUserName'], ENT_QUOTES, 'UTF-8');
				$id = (int) $data['id'];
				$uploaded = (is_null($data['uploaded'])) ? 0 : (int)$data['uploaded'] ;

				echo "<div class='uploader-info-container dd_0_".$id."'> 
						  <div class='uploader-info-pic'><img src='/rei-assets/avatars/".$image."' style='width: 100%; height: 100%; border-radius: 100%;'></div>
						  <div class='uploader-info'> <span>(".$username.")</span> <span>Качвания: ".$uploaded."</span> <span>Присъед.: ".$signUpDate."</span> </div>
					  </div>";
			}
		}
		echo "</section>";
	}

}

?>
