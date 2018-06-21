<?php 
require_once $_SERVER['DOCUMENT_ROOT']."/R_chan/db/db_config.php";
require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php";

/*
 * @author Antonio Drandarov (scVnner)
 * 
 */

class UserInfo extends getDatabase{
	
	protected $userName, $sinedUpDate, $id, $downloads, $views, $scored, $commentCount, $uploadsCount, $favsCount, $followingCount;

	public function user_cover_info(){
		$this->id = intval(User::scriptName("php"));
		$getInfoQ = "SELECT username, signUpDate, p_image FROM uzer WHERE id=?";
		$infoQuery = $this->conn->prepare($getInfoQ);
		$infoQuery->bind_param('i', $this->id);
		$infoQuery->execute();

		$resp1 = @$infoQuery->get_result();
		$data1 = @$resp1->fetch_assoc();
		
		if( is_null($data1['p_image']) ){
			echo "<img id='cover-profile-image' src='/rei-assets/platige-image-reel-2016.jpg'>";
		} else {	
			echo "<img id='cover-profile-image' src='/rei-assets/avatars/".htmlentities($data1['p_image'], ENT_QUOTES, 'UTF-8')."'>";
		}

		echo "<span id='cover-user-name'>".htmlentities($data1['username'], ENT_QUOTES, 'UTF-8')."</span>";		
		echo "<span id='joined-date'>Присъединен: ".htmlentities($data1['signUpDate'], ENT_QUOTES, 'UTF-8')."</span>";
	}



	public function additional_user_info(){
		$getMoreData = "SELECT downloads, views, scored FROM uzer WHERE id=?";	

		$getData1 = $this->conn->prepare($getMoreData);
		$getData1->bind_param('i', $this->id);
		$getData1->execute();

		$resp2 = @$getData1->get_result();
		$data2 = @$resp2->fetch_assoc();

		$this->downloads = ( is_null($data2['downloads']) ) ? 0 : intval($data2['downloads']);
		$this->views = ( is_null($data2['views']) ) ? 0 : intval($data2['views']);
		$this->scored = ( is_null($data2['scored']) ) ? 0 : intval($data2['scored']);
		
		$this->getCommentsAndUploadsCount();
		$this->getFavsAndFollowingsCount();

		echo "<div class='bottom-navbar-containers'><span class='bottom-navbar-content-elements'>Сваляния(".$this->downloads.")</span></div>";
		echo "<div class='bottom-navbar-containers'><span class='bottom-navbar-content-elements'>Гледания(".$this->views.")</span></div>";
		echo "<div class='bottom-navbar-containers'><span class='bottom-navbar-content-elements'>Оценки(".$this->scored.")</span></div>";
	}

	
	
	public function getCommentsAndUploadsCount(){
		$this->commentCount = null;
		$this->uploadsCount = null;
		$dataArr = array();
		$getComCount = "SELECT COUNT(*) AS t FROM comments WHERE fromUserID=? UNION ALL SELECT COUNT(*) AS p FROM posts WHERE fromUserID=?";

		$getData2 = $this->conn->prepare($getComCount);
		$getData2->bind_param('ii', $this->id, $this->id);
		$getData2->execute();

		$resp3 = @$getData2->get_result();
		while( $data3 = @$resp3->fetch_assoc() ){
			foreach($data3 as $key => $value){
				array_push($dataArr, $value);
			}
		}

		$this->commentCount = ( $dataArr[0] == 0 ) ? 0 : intval($dataArr[0]);
		$this->uploadsCount = ( $dataArr[1] == 0 ) ? 0 : intval($dataArr[1]);
			
		echo "<div class='bottom-navbar-containers'><span class='bottom-navbar-content-elements'>Коментари(".$this->commentCount.")</span></div>";
		echo "<div class='bottom-navbar-containers'><span class='bottom-navbar-content-elements'>Качвания(".$this->uploadsCount.")</span></div>";
	}



	public function getFavsAndFollowingsCount(){
		$countArray = array();	
		$this->favsCount = null;
		$this->followingCount = null;
		$getFavsFollowsCount = "SELECT COUNT(*) AS p FROM favourites WHERE fromUserID=? UNION ALL SELECT COUNT(*) AS ff FROM followings WHERE followerID=?";
		
		$getCount = $this->conn->prepare($getFavsFollowsCount);
		$getCount->bind_param('ii', $this->id, $this->id);
		$getCount->execute();

		$countResp = @$getCount->get_result();
		while($countData = @$countResp->fetch_assoc()){
			foreach($countData as $key => $value){
				array_push($countArray, $value);
			}
		}

		if($countArray[0] == 0){
			$this->favsCount = 0;
		} else {
			$this->favsCount = (int) $countArray[0];
		}

		$this->followingCount = ($countArray[1] == 0) ? 0 : (int) $countArray[1];
		
		echo "<div class='bottom-navbar-containers'><span class='bottom-navbar-content-elements'>Любими(".$this->favsCount.")</span></div>";
		echo "<div class='bottom-navbar-containers'><span class='bottom-navbar-content-elements'>Следвания(".$this->followingCount.")</span></div>";
	}



	public function navbar_user_session( $session ){
		$getPicName = "SELECT username, p_image FROM uzer WHERE id=?";
		$get = $this->conn->prepare($getPicName);	
		$get->bind_param('i', $session);		
		$get->execute();

		$respa = @$get->get_result();
		$dataz = @$respa->fetch_assoc();

		$name = htmlentities($dataz['username'], ENT_QUOTES, 'UTF-8');

		if( is_null($dataz['p_image']) ){
			$image = htmlentities("/rei-assets/platige-image-reel-2016.jpg", ENT_QUOTES, 'UTF-8');
		} else {
			$image = htmlentities("/rei-assets/avatars/".$dataz['p_image'], ENT_QUOTES, 'UTF-8');
		}

		echo "<div class='navbar-user-info' id='navbar-redirect-user'><img id='profile-image-top-right' src='".$image."'><span id='navbar-user-name'>".$name."</span></div>";
	}



	public function showInputData( $inputName , $scriptId){
		$getData = "SELECT username, name, birth, email, facebook, twitter, github, skype, instagram FROM uzer WHERE id=?";
		$datttta = $this->conn->prepare($getData);
		$datttta->bind_param('i', $scriptId);
		$datttta->execute();

		$dd = @$datttta->get_result();
		$data = @$dd->fetch_assoc();

		$username = htmlentities($data['username'], ENT_QUOTES, 'UTF-8');
		$email = htmlentities($data['email'], ENT_QUOTES, 'UTF-8');
		
		if(is_null($data['birth'])){
			$birth = "Няма";
		} else {
			$birth = htmlentities($data['birth'], ENT_QUOTES, 'UTF-8');
		}

		$facebook = ( is_null($data['facebook']) ) ? "@facebook" : htmlentities($data['facebook'], ENT_QUOTES, 'UTF-8');
		$twitter = ( is_null($data['twitter']) ) ? "@twitter" : htmlentities($data['twitter'], ENT_QUOTES, 'UTF-8');
		$github = ( is_null($data['github']) ) ? "@github" : htmlentities($data['github'], ENT_QUOTES, 'UTF-8');
		$skype = ( is_null($data['skype']) ) ? "@skype" : htmlentities($data['skype'], ENT_QUOTES, 'UTF-8');
		$instagram = ( is_null($data['instagram']) ) ? "@instagram" : htmlentities($data['instagram'], ENT_QUOTES, 'UTF-8');

		if($inputName == "birth"){
			return $birth;
		} 
		else if($inputName == "username"){
			return $username;
		}
	    else if($inputName == "email"){
			return $email;
		}	
		else if($inputName == "facebook"){
			return $facebook;
		}
	    else if($inputName == "twitter"){
			return $twitter;
		}	
	    else if($inputName == "github"){
			return $github;
		}	
	    else if($inputName == "skype"){
			return $skype;
		}	
	    else if($inputName == "instagram"){
			return $instagram;
		}	
	}


	public function getUserFollowers( $userId ){
		$getFollowerzzz = "SELECT followerName FROM followings WHERE followedID=?";
		$getFoll = $this->conn->prepare($getFollowerzzz);
		$getFoll->bind_param('i', intval($userId));
		$getFoll->execute();
		
		$resp = @$getFoll->get_result();
		while($data = @$resp->fetch_assoc()){
			if(!is_null($data['followerName'])){
				echo "<span class='user-follower-name'>".htmlentities($data['followerName'], ENT_QUOTES, 'UTF-8')."</span>";
			}
		}

	}



	public function getAllUserFollowers( $userId ){
		$getAllFollowers = "SELECT uzer.p_image, followings.followerName FROM uzer RIGHT JOIN followings ON uzer.username = followings.followerName where followedId=?";
		$getFollowers = $this->conn->prepare($getAllFollowers);
		$getFollowers->bind_param('i', $userId);
		$getFollowers->execute();

		$resp = @$getFollowers->get_result();
		echo "<div id='inner-followers-container'>";
		while($data = @$resp->fetch_assoc()){
			if(is_null($data['p_image'])){
				$image = htmlentities("/rei-assets/platige-image-reel-2016.jpg", ENT_QUOTES, 'UTF-8');
			} else {
				$image = htmlentities("/rei-assets/avatars/".$data['p_image'], ENT_QUOTES, 'UTF-8');
			}
			$name = htmlentities($data['followerName'], ENT_QUOTES, 'UTF-8');
			echo "<div class='follower-element'><img class='follower-image' src='".$image."'> <span class='the-follower-name'>".$name."</span> </div>";
		}
		echo "</div>";
	}

	
	public function getUserPostsTags( $userId ){
		$tagsArr = array();
		$getAllTags = "SELECT tagz FROM posts WHERE fromUserID=?";	
		$getTags = $this->conn->prepare($getAllTags);
		$getTags->bind_param('i', $userId);
		$getTags->execute();

		$resp = @$getTags->get_result();
		while($data = @$resp->fetch_assoc()){
			$tagz = explode(' ', $data['tagz']);
			foreach($tagz as $tag){
				array_push($tagsArr, $tag);
			}
		}

		foreach(array_unique($tagsArr) as $tag){
			if($tag !== ""){
				echo "<span class='post-tag'>".htmlentities($tag, ENT_QUOTES, 'UTF-8')."</span>";	
			}
		}
	}
	

	public function getProfileImageToUpdate( $userId ){
		$getImage = "SELECT p_image FROM uzer WHERE id=?";		
		$getPic = $this->conn->prepare($getImage);
		$getPic->bind_param('i', $userId);
		$getPic->execute();

		$resp = @$getPic->get_result();
		$data = @$resp->fetch_assoc();

		if(is_null($data['p_image'])){
			$image = htmlentities("/rei-assets/platige-image-reel-2016.jpg", ENT_QUOTES, 'UTF-8');
		} else {
			$image = htmlentities("/rei-assets/avatars/".$data['p_image'], ENT_QUOTES, 'UTF-8');
		}

		return $image;
	}


	public function getAllUserPosts( $userId ){
		$getPostz = "SELECT posts.postName, posts.fromUserName, posts.favourited, posts.id, uzer.p_image FROM posts RIGHT JOIN uzer ON posts.fromUserID = uzer.id where fromUserID=?";
		$getPosts = $this->conn->prepare($getPostz);
		$getPosts->bind_param('i', $userId);
		$getPosts->execute();

		$resp = @$getPosts->get_result();
		while($postData = @$resp->fetch_assoc()){
			$post ="/R_chan/posts/".$postData['postName'];
			$postId = (int) $postData['id'];
			$uploaderName = htmlentities($postData['fromUserName'], ENT_QUOTES, 'UTF-8');
			$userPic = (is_null($postData['p_image'])) ? "/rei-assets/platige-image-reel-2016.jpg" : "/rei-assets/avatars/".$postData['p_image'] ;
			echo "<div class='the-user-uploaded-post ".$postId."'> <div class='top-post-img-info'> <img class='the-post-uploader-pic' src='".htmlentities($userPic, ENT_QUOTES, 'UTF-8')."'> <span class='the-post-uploader-name'>".$uploaderName."</span> <i class='fa fa-heart fav-post-count'></i> <i class='fa fa-flag post-flagged'></i> </div> <img class='inner-img-post' src='".htmlentities($post, ENT_QUOTES, 'UTF-8')."'></div>";
		}
	}	
}


?>
