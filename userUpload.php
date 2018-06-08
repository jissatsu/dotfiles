<?php  @session_start(); ?>
<?php require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php"; ?>
<?php if(!User::session("uzer", "id") && !User::session("uzer", "log_name")){User::redirect("/");}  ?>
<?php require_once __DIR__."/user_info1.php"; $info1 = new UserInfo(); $info1->db_conf();?>
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
	<link rel="stylesheet" type="text/css" href="/rei-assets/css/the-upload-page.css">	
	<link rel="stylesheet"href="https://fonts.googleapis.com/icon?family=Material+Icons">
	<link rel="stylesheet" href="/rei-assets/css/bootstrap.min.css">	

	<script type='text/javascript'>
		var head = document.getElementsByTagName('head')[0];
		var title = document.createElement('title');
		title.appendChild(document.createTextNode('cyber-chann.bg | upload'));
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
							<a href='/' id='terms-redirect'>Условия</a>
							<a href='/' id='uploaders-redirect'>Ъплоудъри</a>
							<a href='/' id='tags-redirect'>Тагове</a>
							<a href='/' id='gallery-redirect'>Галерия</a>
							<a href='/' id='upload-redirect'>Качване</a>
							<a href='/' id='logout-redirect'>Изход</a>
						</div>";
				}
			?>
			
			<div id='hor'></div>
			<section class='user-file-upload-section'>
				<div id='post-container'>
					<img src='/rei-assets/Upload-Transparent-Images.png' id='inner-upload-image'>
					<img src='' id='output'>
				</div>	
				<div id='post-info-container'>
					<?php 	
						$fileUploader = new FileUploader(array("maxFileSize" => 1000000, "path" => $_SERVER['DOCUMENT_ROOT']."/R_chan/posts/", "button" => "submit-all", 
													"forbidden_extensions" => array("cpp", "c", "py", "sh", "zsh", "php", "rb", "xml", "css", "js", "html", "csv", "txt", "pdf", "ksh"), 
													"allowed_extensions" => array("jpg", "png", "jpeg", "gif"), 
													"database_table" => "posts",
													"query_type" => "insert",
													"table_column" => "", 
													"where_column" => ""));
					
					echo "<form action='".$fileUploader->ProcessFileData()."' method='post' enctype='multipart/form-data'>";
					?>
						<div id='post-info-inner-container-one'>

							<div id='post-info-file-container'>
								<label for='myFile' class='file-label'>Избери</label>
								<input type='file' name='file' id='myFile' accept='image/jpg, image/jpeg, image/png, image/gif' onchange='openFile(event)'>	
								<div id='post-info-inner-file-container'>
									<span id='post-info-file'>Файл</span>
								</div>
							</div>
	
							<div id='post-info-category-container'>
								<span id='gif-text'>Gif</span> <input type='radio' name='gif' id='gif' value='giff'>
								<span id='image-text'>Image</span> <input type='radio' name='image' id='image' value='immage'>
								<div id='post-info-inner-category-container'>
									<span id='post-info-category'>Категория</span>
								</div>
							</div>

							<div id='post-info-source-container'>
								<input type='text' name='source-input' id='source-input' placeholder='http://some-source.com'>
								<div id='post-info-inner-source-container'>
									<span id='post-info-source'>Източник</span>
								</div>
							</div>

							<div id='post-info-tags-container'>	
								<input type='text' name='tags-input' id='tags-input' placeholder='tag1, tag2, tag3'>
								<div id='post-info-inner-tags-container'>
									<span id='post-info-tags'>Тагове</span>
								</div>
							</div>

							<div id='post-info-rating-container'>
								<span id='safe-text'>Safe</span> <input type='radio' name='safe' id='safe' value='safe'>
								<span id='explicit-text'>Explicit</span> <input type='radio' name='explicit' id='explicit' value='explicit'>
								<div id='post-info-inner-rating-container'>
									<span id='post-info-rating'>Рейтинг</span>
								</div>
							</div>
							<input class='save-button-2' type='submit' name='submit-all' id='submit-all' value='Запазване'>
						</div>
					</form>
				</div>
			</section>
	
</div>	

</body>


<script type='text/javascript' src="http://code.jquery.com/jquery-2.0.3.min.js"></script>
<script type='text/javascript' src="/rei-assets/js/bootstrap.min.js"></script>
<script type='text/javascript' src="/rei-assets/js/the-upload-page.js"></script>

<script type='text/javascript'>

var openFile = function(event){
	var input = event.target;
	
	var reader = new FileReader();
	reader.onload = function(){
		var dataURL = reader.result;
		var output = $('#output');
		
		var image = new Image();
		image.onload = function(){
			if(this.width - this.height > 100){
				$('#post-container').css('width', '46%');				
			} else if(this.height - this.width > 100){
				$('#post-container').css('height', '90%');					
			}
		};

		output.attr('src', dataURL);
		output.css('opacity', 1);
		image.src = dataURL;
	};
	
	reader.readAsDataURL(input.files[0]);	
};

let navUser = $('#navbar-user-name').html(); let newNavUser = ""; if(navUser.length >= 12){newNavUser = navUser.replace(navUser.substring(12), "...");$('#navbar-user-name').html(newNavUser);}
</script>

<footer>
	<div id='footer-inner-wrapper'>
		<span id='author'>By: Antonio Drandarov (scVnner)</span>
		<span id='copyright'>&copy; Copyright 2018 cyber-chann</span>
		<span id='find-me'> <i id='git-icon' class='fa fa-github'></i> Find me on: <a href='https://github.com/JissatsU' id='git-hub'>@JissatsU</a> </span>
		<span id='inspired-by'>Inspired by: <a href='https://wallhere.com/'>wallhere.com, </a> <a href='https://www.artstation.com/'>artstation.com</a></span>
	</div>
</footer>

</html>

