<?php @session_start(); ?>
<?php require_once __DIR__."/user_info1.php"; $info1 = new UserInfo(); $info1->db_conf();?>
<?php require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php"; ?>
<?php require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/FileUploader.php"; ?>

<!DOCTYPE html>
<html lang='bg'>
<head>
	
	<meta http-equiv="content-type" content="text/html; charset=utf-8">
	<meta http-equiv="Cache-control" content="no-cache">
	<meta charset="utf-8">
	<meta name="language" content="BG">
	<meta name="subject" content="Anime, Cyberpunk etc...">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<meta name="author" content="Antonio Drandarov (scVnner)">
	<meta name="keywords" content="anime, cyberpunk, art, cyber-chann, cyber-chan">
	<meta name="classification" content="Art">	
	<meta property="og:description" content="Ако ви е скучно и нямате какво друго да правите, това е мястото за вас."/>
	<meta name="copyright" content="Copyright 2018 cyber-chann">	

	<meta property="og:locale" content="bg">
	
	<meta property="og:type" content="website" />
	<meta property="og:title" content="www.cyber-chann.bg" /> 
	<meta property="og:image" content="www.cyber-chann.bg/rei-assets/e.jpg" />
	<meta property="og:image:type" content="image/jpeg" />
	<meta property="og:url" content="http://www.cyber-chann.bg" />
	<meta property="og:image:width" content="400" />
	<meta property="og:image:height" content="300" />
	<link rel="shortcut icon" href="/rei-assets/e.jpg" type="image/x-icon">

	<link rel="stylesheet" type="text/css" href="/Content/Content/font-awesome.min.css">
	<link rel='stylesheet' type='text/css' href='/rei-assets/css/full-slider.css'>	
	<link rel="stylesheet" type="text/css" href="/rei-assets/css/the-profile.css">	
	<link rel="stylesheet"href="https://fonts.googleapis.com/icon?family=Material+Icons">
	<link rel="stylesheet" href="/rei-assets/css/bootstrap.min.css">	

	<script type='text/javascript'>
		var head = document.getElementsByTagName('head')[0];
		var title = document.createElement('title');
		title.appendChild(document.createTextNode('cyber-chann.bg | cyber-chann.bg Wallpapers & Gifs'));
		head.appendChild(title);
	</script>	
</head>

<body>

	<div class='main-user-profile-container'>
		<div class='user-profile-navbar'>
			<img id='the-site-logo' src='/rei-assets/e.jpg'>	
			<span id='the-site-name'>CYBER-CHANN</span>
			<div class='search-box'>	
				<form method='get' action=''>
					<input type='text' id='search-posts-field' name='search-post' placeholder='Търсене . . . '>
					<div id='search-ico-container'>
						<i id='search-icon' class='fa fa-search' aria-hidden="true"></i>
					</div>
					<input type='submit' id='search-button' name='search-button' value>
				</form>
			</div>
			<?php 

				if( (User::session( "uzer", "log_name" )) && (User::session( "uzer", "id" )) ){
					$info1->navbar_user_session( User::session( "uzer", "id" ) );
				} else {
					echo "<div class='navbar-user-info-redirect'><img id='profile-image-top-right' src='/rei-assets/platige-image-reel-2016.jpg'><span id='log-in'>Влез сега!</span></div>";
				}

			?>
			<i id='menu-bars' class='fa fa-bars' aria-hidden="true"></i>
		</div>	
			<?php 
				echo "<div id='dark-screen'>";
				if( (User::session("uzer", "log_name") && User::session("uzer", "id")) && User::session("uzer", "id") == User::scriptName("php") ){
					echo "	<section id='manage-settings-container'>
								<div id='settings-container-upper'><span id='settings-container-upper-text'>Редактиране</span></div> 
								<div id='inner-manage-settingz-container'>
									<span id='name-show'>Име</span> 
									<input type='text' name='change-name' id='change-name' value='".$info1->showInputData("name", User::scriptName("php"))."'> 								  								  <input type='text' name='change-birth' id='change-birth' value='".$info1->showInputData("birth", User::scriptName("php"))."'> 
									<input type='text' name='change-user-name' id='change-user-name' value='".$info1->showInputData("username", User::scriptName("php"))."'>  
									<span id='birth-date-show'>Дата на раждане</span> 
									<span id='user-name-show'>Потр. име</span> 
									<div id='ver1'></div> 
									<span id='contact'>Контакт</span>   
									<span id='github'> <i class='fa fa-github'></i> github</span> 
									<input type='text' name='github' id='github-input' value='".$info1->showInputData("github", User::scriptName("php"))."'>    
									<span id='twitter'> <i class='fa fa-twitter'></i> twitter</span>    
									<input type='text' name='twitter' id='twitter-input' value='".$info1->showInputData("twitter", User::scriptName("php"))."'>    
									<span id='facebook'> <i class='fa fa-facebook'></i> facebook</span>    
									<input type='text' name='facebook' id='facebook-input' value='".$info1->showInputData("facebook", User::scriptName("php"))."'>    
									<span id='skype'> <i class='fa fa-skype'></i> skype</span>    
									<input type='text' name='skype' id='skype-input' value='".$info1->showInputData("skype", User::scriptName("php"))."'>      
									<span id='instagram'> <i class='fa fa-instagram'></i> instagram</span>    
									<input type='text' name='instagram' id='instagram-input' value='".$info1->showInputData("instagram", User::scriptName("php"))."'>   
									<button id='save-button-1'>Запазване</button>    
								</div>
								<div id='update-profile-image-container'>
									<span id='update-profile-image-text'>Профилна снимка</span>
									<div id='user-profile-image-container'><img id='output' src='".$info1->getProfileImageToUpdate( (int) User::scriptName("php") )."'></div>";
					
							$fileUploader = new FileUploader(array("maxFileSize" => 80000, "path" => $_SERVER['DOCUMENT_ROOT']."/rei-assets/avatars/", "button" => "submit_upload", 
														"forbidden_extensions" => array("cpp", "c", "py", "sh", "zsh", "php", "rb", "xml", "css", "js", "html", "csv", "txt", "pdf", "ksh"), 
														"allowed_extensions" => array("jpg", "png", "jpeg"), 
														"database_table" => "uzer",
														"query_type" => "update",
														"table_column" => "p_image", 
														"where_column" => "id"));
					
								echo "
									<form action='".$fileUploader->ProcessFileData()."' method='POST' enctype='multipart/form-data'>
										<label for='file-upload-input' class='file-label'>Избиране</label>
										<input id='file-upload-input' type='file' name='file' id='file' onchange='openFile(event)' accept='image/jpeg, image/png, image/jpg'>
										<input class='save-button-2' type='submit' name='submit_upload'>
									</form>
								</div>
							</section>";
				}
				echo "<section id='followers-container'><div id='followers-container-upper'><span id='followers-container-upper-text'>Последователи</span></div>";
				$info1->getAllUserFollowers( (int) User::scriptName("php") );
				echo "</section>";
				echo "</div>";
			?>
			<?php 
				if( (User::session( "uzer", "log_name" )) && (User::session( "uzer", "id" )) ){
				
					echo "<div class='navbar-dropdown-menu'>
							<a href='/user/".User::session('uzer', 'id')."' id='profile-redirect'>Профил</a>
							<a href='#' id='terms-redirect'>Условия</a>
							<a href='#' id='uploaders-redirect'>Ъплоудъри</a>
							<a href='#' id='tags-redirect'>Тагове</a>
							<a href='#' id='gallery-redirect'>Галерия</a>
							<a href='/user/upload' id='upload-redirect'>Качване</a>
							<a href='/user/logout' id='logout-redirect'>Изход</a>
						</div>";
				} else {
					
					echo "<div class='navbar-dropdown-menu'>
							<a href='/' id='profile-redirect'>Профил</a>
							<a href='#' id='terms-redirect'>Условия</a>
							<a href='#' id='uploaders-redirect'>Ъплоудъри</a>
							<a href='#' id='tags-redirect'>Тагове</a>
							<a href='#' id='gallery-redirect'>Галерия</a>
							<a href='/user/upload' id='upload-redirect'>Качване</a>
							<a href='/' id='logout-redirect'>Изход</a>
						</div>";
				}
			?>

		<section class='profile-cover-background-image'>
			<?php $info1->user_cover_info(); ?>	
			<span id='cover-clock'></span>
			<?php  
				if( (User::session("uzer", "log_name") && User::session("uzer", "id")) && User::session("uzer", "id") == User::scriptName("php")){
					echo "<button id='user-profile-settings'>Редактиране</button>";	
				} else {
					echo "<button id='send-message'>Изпрати съоб.</button>";
					echo "<button id='follow-user'>Последвай</button>";
				}
			?>
		</section>

		<section class='additional-user-info'>
			<div class='inner-additional-user-info-container'>
				<?php $info1->additional_user_info(); ?>	
			</div>	
		</section>
		
		<section class='more-user-info-sections'>
			<div id='tags-button'>Тагове</div>
			<div id='tags-inner-container'></div>			
			<div id='followers-button'>Последователи</div>
			<div id='followers-inner-container'>
				<?php  $info1->getUserFollowers( User::scriptName("php") ); ?>
			</div>
		</section>
		
		<div id='vertical1'></div>
		<section class='user-uploaded-posts-container'></section>

	</div>	

</body>

<script type='text/javascript' src="http://code.jquery.com/jquery-2.0.3.min.js"></script>
<script type='text/javascript' src="/rei-assets/js/bootstrap.min.js"></script>
<script type='text/javascript' src="/rei-assets/js/the-profile.js"></script>

<script type="text/javascript">

var openFile = function(event){
	var input = event.target;
	
	var reader = new FileReader();
	reader.onload = function(){
		var dataURL = reader.result;
		var output = $('#output');
		
		var image = new Image();
		image.onload = function(){
			if(this.width > 600 && this.height > 650){
				$('.save-button-2').prop('disabled', true);
				$('.save-button-2').css('cursor', 'not-allowed');
			} else {
				output.attr('src', dataURL);
				$('.save-button-2').prop('disabled', false);
				$('.save-button-2').css('cursor', 'pointer');	
			}
		};
		
		image.src = dataURL;
	};
	reader.readAsDataURL(input.files[0]);	
};

$.ajax({url: "/uss",method: "post",async: true,success: function(result){var sName = result.split(',')[0]; var id = result.split(',')[1]; var sId = document.URL.split('/');
	if($('#follow-user')){
		$('#follow-user').click(function(){
			if(!sName && !id){
				window.location.href = "/";
			} else {
				$.ajax({url: "/fuu", method: "post", async: true, data: "data="+JSON.stringify({"sName":sName, "sId":id, "toId":sId[sId.length - 1]}), success: function(resp){null;}});
			}
		}
)}; 
if($('#send-message')){$('#send-message').click(function(){if(!sName && !id){window.location.href = "/";}})};if(id == sId[sId.length - 1]){if(window.navigator){var os = window.navigator.platform;if((/win/i).exec(os)){$('.profile-cover-background-image').append('<span id="user-os-info"> os: <i class="fa fa-windows"></i> windows</span>');} else if((/linux/i).exec(os)){$('.profile-cover-background-image').append('<span id="user-os-info"> os: <i class="fa fa-linux"></i> linux </span>');	}}}}	}); let navUser = $('#navbar-user-name').html(); let newNavUser = ""; if(navUser.length >= 12){newNavUser = navUser.replace(navUser.substring(12), "...");$('#navbar-user-name').html(newNavUser);}
</script>

</html>




