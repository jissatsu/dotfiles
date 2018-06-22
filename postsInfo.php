<?php 
require_once $_SERVER['DOCUMENT_ROOT']."/R_chan/db/db_config.php";
require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php";

/*
 * @author Antonio Drandarov (scVnner)
 */

class Posts extends getDatabase{

		
	public function countPosts(){
		$getCount = "SELECT COUNT(*) AS cc FROM posts";	
		$getCC = $this->conn->query($getCount);

		$data = @$getCC->fetch_assoc();

		$this->count = (int) $data['cc'];

		echo "<span id='post-count'>~".$this->count."</span>";
	}


	public function getAllTagz(){
		$tagzz = array();
		$getTags = "SELECT tagz FROM posts";	
		$get = $this->conn->query($getTags);	
		while($result = @$get->fetch_assoc()){
			$tagg = htmlentities($result['tagz'], ENT_QUOTES, 'UTF-8');	
			foreach(explode(' ', $tagg) as $tag){
				if($tag !== ""){
					array_push($tagzz, $tag);
				}
			}
		}
		foreach(array_unique($tagzz) as $tag){
			echo "<span class='the-tag'>".$tag."</span>";
		}
	}


	public function getEverySinglePost(){
		$getEveryPost = "SELECT posts.postName, posts.fromUserName, posts.favourited, posts.id, uzer.p_image 
		                     	FROM posts 
		                    	RIGHT JOIN uzer 
                             	ON posts.fromUserID = uzer.id LIMIT ? OFFSET ?";
		$limit = 18;
		$Offset = 0;
		$numberOfSections = ceil($this->count / $limit);

		for($section = 1 ; $section <= $numberOfSections ; $section++){

			$Offset = ($section - 1) * $limit;
			
			$getPosts = $this->conn->prepare($getEveryPost);
			$getPosts->bind_param('ii', $limit, $Offset);
			$getPosts->execute();

			$resp = @$getPosts->get_result();
			
			echo "<div class='section-num'><span>".$section."</span></div>";
			echo "<section class='inner-posts-container'>";		

			while($data = @$resp->fetch_assoc()){
				$postName = "/R_chan/posts/".htmlentities($data['postName'], ENT_QUOTES, 'UTF-8');
				$uploaderName = htmlentities($data['fromUserName'], ENT_QUOTES, 'UTF-8');
				$uploaderPic = ( is_null($data['p_image']) ) ? "/rei-assets/platige-image-reel-2016.jpg" : "/rei-assets/avatars/".htmlentities($data['p_image'], ENT_QUOTES, 'UTF-8');

				echo "<div class='the-inner-post ".(int) $data['id']."'>
						  <div class='top-image-info'><img class='uploader-avatar' src='".$uploaderPic."'><span class='uploader-name'>".$uploaderName."</span><i class='fa fa-heart fav-post-count'></i><i class='fa fa-flag'></i></div>
						  <img class='inner-img-post' src='$postName' style='width: 100%; height: 100%;'>
					  </div>";
			}
			
			echo "</section>";
		}
	}

}

?>
