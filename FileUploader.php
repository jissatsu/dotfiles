<?php 
require_once $_SERVER['DOCUMENT_ROOT']."/R_chan/db/db_config.php";
require_once __DIR__."/HelperClass.php";

/*
 * @author Antonio Drandarov (scVnner)
 *
 */

class FileUploader extends getDatabase{
	
	public $maxFileSize, $path, $button;
	protected $fileName, $fileSize, $fileError, $fileExtension, $fileType, $fileTmp;	

	public function __construct($params = array()){
		$this->maxFileSize = (int) $params['maxFileSize'];
		$this->path = User::sanitizeInput($params['path']);
		$this->button = User::sanitizeInput($params['button']);
		$this->nExtensions = User::sanitizeInput($params['forbidden_extensions']);
		$this->yExtensions = User::sanitizeInput($params['allowed_extensions']);
		$this->databaseTable = User::sanitizeInput($params['database_table']);
		$this->tableColumn = User::sanitizeInput($params['table_column']);
		$this->queryType = User::sanitizeInput($params['query_type']);
		$this->whereColumn = User::sanitizeInput($params['where_column']);

		$this->db_conf();
	}


	public function ProcessFileData(){
		if(User::requestMethod("post")){
			if(isset($_POST[$this->button])){
				if(is_null($_FILES['file']) || $_FILES['file']['name'] == "" || $_FILES['file']['size'] == 0){
					echo "<span class='file-error'>Моля изберете файл!</span>";
				} else {
					$this->fileName = User::sanitizeInput($_FILES['file']['name']);
					$this->fileTmp = User::sanitizeInput($_FILES['file']['tmp_name']);
					$this->fileSize = User::sanitizeInput($_FILES['file']['size']);
					$this->fileError = User::sanitizeInput($_FILES['file']['error']);
					$this->fileType = User::sanitizeInput($_FILES['file']['type']);

					$this->ProcessFileUpload();
				}
			}	
		}
	}


	public function ProcessFileUpload(){
		$fileDotSplit = explode(".", $this->fileName);
		$this->fileExtension = strtolower(end($fileDotSplit));
		$fileType = finfo_open(FILEINFO_MIME_TYPE);
		$fileImageType = finfo_file($fileType, $this->fileTmp);

		$gotFileInfo = explode("/", $fileImageType);	

		for($i = 0 ; $i < count($this->nExtensions) ; $i++){
			if(in_array($this->nExtensions[$i], $fileDotSplit)){
				echo "<span id='file-error'>Форматът на файла не се поддържа!</span>";
				break;
			} else {
				if(!in_array($this->fileExtension, $this->yExtensions)){
					echo "<span id='file-error'>Форматът на файла не се поддържа!</span>";
				} else {
					if($gotFileInfo[0] !== "image"){
						echo "<span id='file-error'>Файлът не е картинка!</span>";
					} else {
						if(!in_array($gotFileInfo[1], $this->yExtensions)){
							echo "<span id='file-error'>Форматът на файла не се поддържа!</span>";
						} else {
							if($this->fileSize > $this->maxFileSize){
								echo "<span id='file-error'>Размерът на файла е твърде голям!</span>";
							} else {
								switch($this->queryType){
									case "update":
										$query = "UPDATE $this->databaseTable SET $this->tableColumn=? WHERE $this->whereColumn=?";
										$p_image_name = uniqid().uniqid().(string) rand(1,200).$this->fileName;
										if(is_uploaded_file($this->fileTmp)){
											$updateImage = $this->conn->prepare($query);
											$updateImage->bind_param('si', $p_image_name, intval(User::session("uzer", "id")));
											$updateImage->execute();
											
											$file = $_SERVER['DOCUMENT_ROOT']."/R_chan/users.xml";

											$doc = new DOMDocument();
											$xml = file_get_contents($file);
											
								            $doc->loadXML($xml);
											
											$root = $doc->firstChild;
											
											$user = $root->getElementsByTagName('user');

											foreach($user as $userNode){
												if(intval($userNode->getAttribute('id')) == intval(User::session("uzer", "id"))){
													if($picture = $userNode->getElementsByTagName('picture')->item(0)){
														$userNode->removeChild($picture);			
													}
													
													$picture = $doc->createElement('picture');
													$picture->appendChild($doc->createTextNode($p_image_name));
													$userNode->appendChild($picture);
												} 
											}

											$doc->save($file);
											
											move_uploaded_file($this->fileTmp, $this->path.$p_image_name);
										}		
									break;

									case "insert":
										$typeGif = User::sanitizeInput($_POST['gif']);			
										$typeImage = User::sanitizeInput($_POST['image']);	
										$safe = User::sanitizeInput($_POST['safe']);
										$explicit = User::sanitizeInput($_POST['explicit']);
										$source = User::sanitizeInput($_POST['source-input']);
										$tags = User::sanitizeInput($_POST['tags-input']);

										$resolution = getimagesize($this->fileTmp)[0] . "x" . getimagesize($this->fileTmp)[1];
										$the_image_name = uniqid().uniqid().(int) rand(1,200).$this->fileName;
										$fromUserName = User::sanitizeInput(User::session("uzer", "log_name"));

										if($typeGif !== "" && $typeImage == ""){
											$category = "gif";
										} else if($typeGif == "" && $typeImage !== ""){
											$category = "image";	
										} else {
											$category = null;
										}

										if($safe !== "" && $explicit == ""){
											$rating = "safe";
										} else if($safe == "" && $explicit !== ""){
											$rating = "explicit";	
										} else {
											$rating = null;
										}
										
										$source = ($source == "") ? null : $source;
										$tags = ($tags == "") ? null : $tags;

										if(!is_null($tags)){
											if(User::validateTagsFormat($tags) == false){
												echo "<span id='tag-source-err'>Таговете се отделят със запетаи!</span>";
												break;
											} else {
												$tags = preg_replace("/(\,)/", " ", $tags);
											}
										}

										if(!is_null($source)){
											if(!User::validateURLFormat($source)){
												echo "<span id='tag-source-err'>Невалиден формат за източник!</span>";
												break;
											} else {
												$source = $source;
											}
										}
										
										$query = "INSERT INTO posts (postName, fromUserName, fromUserID, resolution, tagz, category, rating, source) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
										if(is_uploaded_file($this->fileTmp)){
											$inserT = $this->conn->prepare($query);
											$inserT->bind_param('ssisssss', $the_image_name, $fromUserName, User::session("uzer", "id"), $resolution, $tags, $category, $rating, $source);
											$inserT->execute();
											move_uploaded_file($this->fileTmp, $this->path.$the_image_name);
										}

									break;
								}								
							}	
						}
					}
				}
				break;
			}
		}		

	}
	
}



?>


