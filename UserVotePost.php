<?php 
@session_start();
require_once __DIR__."/db/db_config.php";
require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php";

/*
 * @author (scVnner)
 */

class VotePost extends getDatabase{
	
	public function __construct(){
		$this->db_conf();
		$this->data = json_decode($_POST['data']);	
		$this->voterID = (int) preg_replace("/([^0-9])/", "", $this->data->vi);
		$this->voterName = preg_replace("/([^0-9a-zA-Z\\s])/", "", $this->data->vn);
		$this->postID = (int) preg_replace("/([^0-9])/", "", $this->data->pi);
		$this->score = (int) preg_replace("/([^0-9])/", "", $this->data->sc);	

		$this->sesID = (int) preg_replace("/([^0-9])/", "", User::session("uzer", "id"));
		$this->sesName = preg_replace("/([^0-9a-zA-Z\\s])/", "", User::session("uzer", "log_name"));

		$this->qqs = array("check-records" => "SELECT id, points FROM votes WHERE voterID=? AND voterName=? AND postID=?",
						   "insert-record" => "INSERT INTO votes (voterID, voterName, postID, points) VALUES (?, ?, ?, ?)",
						   "update-record" => "UPDATE votes SET points=? WHERE voterID=? AND voterName=? AND postID=?",
						   "update-uzer-record" => "UPDATE uzer SET scored=? WHERE id=? AND username=?",
						   "update-posts-table" => "UPDATE posts SET score=? WHERE id=?",
						   "delete-record" => "DELETE FROM votes WHERE voterID=? AND voterName=? AND postID=? AND points=?");
		
		if( ($this->score && $this->postID) && ($this->voterID == $this->sesID && $this->voterName == $this->sesName) ){
			if( !($this->score <= 0 || $this->score > 3) ){
				$this->ProceedPostVoting( $this->postID, $this->score );		
			}
		}
	}


	public function ProceedPostVoting( $onPost, $withScore ){
		$fCheck = $this->conn->prepare($this->qqs['check-records']);	
		$fCheck->bind_param('isi', $this->voterID, $this->voterName, $onPost);
		if($fCheck->execute()){
			$resp = @$fCheck->get_result();
			$data = @$resp->fetch_assoc();

			if(is_null($data)){
				$sCheck = $this->conn->prepare($this->qqs['insert-record']);
				$sCheck->bind_param('isii', $this->voterID, $this->voterName, $onPost, $withScore);
				$sCheck->execute();
			} 
			else if( $withScore !== (int)$data['points'] ){
				$update = $this->conn->prepare($this->qqs['update-record']);
				$update->bind_param('iisi', $withScore, $this->voterID, $this->voterName, $onPost);
				$update->execute();
			}
		   	else {
				$delete = $this->conn->prepare($this->qqs['delete-record']);
				$delete->bind_param('isii', $this->voterID, $this->voterName, $onPost, $withScore);
				$delete->execute();	
			}	
		}
	}

	public function UpdateUserTable(){
		$result = 0 ;
		
		$getScores = "SELECT id, points, postID FROM votes WHERE voterID=? AND voterName=?";			

		$getAll = $this->conn->prepare($getScores);
		$getAll->bind_param('is', $this->sesID, $this->sesName);
		if($getAll->execute()){
			$resp = @$getAll->get_result();
			while($data = @$resp->fetch_assoc()){
				echo $data['points'] . '-' . $data['postID'] . ',';
				$point = (int) $data['points'];
				$result += $point;
			}

			if( ($this->voterID && $this->voterName && $this->postID) && ($this->voterID == $this->sesID && $this->sesName == $this->voterName) ){
				$update = $this->conn->prepare($this->qqs['update-uzer-record']);
				$update->bind_param('iis', $result, $this->voterID, $this->voterName);
				$update->execute();
			}
		}
	}

	public function UpdatePostsTable(){
		$result = 0 ;
		$getScores = "SELECT points FROM votes WHERE postID=?";

		$getAll = $this->conn->prepare($getScores);
		$getAll->bind_param('i', $this->postID);
		if($getAll->execute()){

			$resp = @$getAll->get_result();
			while($data = @$resp->fetch_assoc()){
				$result += (int) $data['points'];
			}
			
			$update = $this->conn->prepare($this->qqs['update-posts-table']);
			$update->bind_param('ii', $result, $this->postID);
			$update->execute();

		}
	}
}

$vote = new VotePost();
$vote->UpdateUserTable();
$vote->UpdatePostsTable();

?>
