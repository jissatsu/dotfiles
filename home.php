<?php @session_start(); ?>
<?php require_once $_SERVER['DOCUMENT_ROOT']."/rei-assets/HelperClass.php"; ?>
<?php //unset($_SESSION['authCode']); unset($_SESSION['authId']); ?>
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
	<link rel="stylesheet" type="text/css" href="/rei-assets/css/home-main.css">	
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

	<div class='main-wrapper-container'>
		<div id='the-carousel' class='carousel slide'>
			<ol class='carousel-indicators'>
				<li class='item1 active'></li>	
				<li class='item2'></li>	
				<li class='item3'></li>	
				<li class='item4'></li>	
			</ol>	
			
			<div class='carousel-inner' role='listbox'>
				<div class='item active'><img src='/rei-assets/sunset-night-robot-futuristic-artwork-evening-512150-wallhere.com.jpg'></div>	
				<div class='item'><img src='/rei-assets/ari-targownik-robot-solo.jpg'></div>	
				<div class='item'><img src='/rei-assets/wallhalla-AZLeGJ7uG3.jpg'></div>	
				<div class='item'><img src='/rei-assets/weapon-cyberpunk-fortress-mountain-snow-winter.jpg'></div>	
			</div>			

		</div>			

		<div class='second-main-wrapper-container'>
			<section id='cookie-notifier'>
				<div id='inner-cookie-notifier-container'>
					<i id='warning' class='fa fa-exclamation-triangle'></i>
					<span id='cookie-info-text'>Бисквитките ви са спрени.<br/>Желателно е да ги пуснете,<br/>за да влизате по лесно.</span>
				</div>
				<div id='inner-cookie-notifier-timer'><span id='the-timer'></span></div>
				<i id='close-window' class='fa fa-times'></i>
			</section>

			<section class='side-menu-bar'>
				<div class='side-menu-bar-item' id='image-item'><img id='logo-image' src='/rei-assets/e.jpg'></div>		
				<div class='side-menu-bar-item' id='search-item'>
					<p id='search'><i id='search-icon' class='fa fa-search'></i></p>	
				</div>		
				<div class='side-menu-bar-item'>
					<p id='home'><i id='home-icon' class='fa fa-home'></i></p>
				</div>		
				<div class='side-menu-bar-item'>
					<p id='posts'><i id='posts-icon' class='fa fa-image'></i></p>
				</div>		
				<div class='side-menu-bar-item'>	
					<p id='members'><i id='members-icon' class='fa fa-users'></i></p>
				</div>		
				<div class='side-menu-bar-item'>
					<p id='envelope'><i id='envelope-icon' class='fa fa-envelope'></i></p>			
				</div>		
				<div class='side-menu-bar-item'>
					<p id='upload'><i id='upload-icon' class='fa fa-upload'></i></p>	
				</div>		
				<div class='side-menu-bar-item'>	
					<p id='comments'><i id='comments-icon' class='fa fa-comments'></i></p>	
				</div>		
				<div class='side-menu-bar-item'>
					<p id='tagz'><i id='tagz-icon' class='fa fa-tags'></i></p>	
				</div>		
			</section>	
		<?php 
			if(!User::session("uzer", "log_name") && !User::session("uzer", "id")){
				echo "
				<section class='login-form-section' id='login-section'>
					<div class='inner-login-form-container'>
						<div class='inner'>
							<div id='login-form-upper'><p id='login-text'>Влизане</p></div>	
							<input type='text' id='username' name='username'>	<input type='password' id='password' name='password'>
							<span id='potrebitel'> <i id='user-ico' class='fa fa-user'></i> Потребител</span>	
							<span id='parola'> <i id='pass-ico' class='fa fa-lock'></i> Парола</span>	
							<a href='/user/password-link-request' id='forgotten-pass'>Забравена парола?</a>
							<button id='login-button'>Вход</button>   <button id='register-button'>Регистрирай се!</button>
							<span id='remember'> <input type='checkbox' id='remember-me'> Запомни ме!</span>
						</div>
					</div>
				</section>			
		
				<section id='register-form'>
					<span id='register-user'> <i id='register-user-ico' class='fa fa-user'></i> Потребител</span>
					<span id='register-email'> <i id='register-email-ico' class='fa fa-envelope'></i> Имейл</span>
					<span id='register-pass1'> <i id='register-pass1-ico' class='fa fa-lock'></i> Парола</span>
					<span id='register-pass2'> <i id='register-pass2-ico' class='fa fa-lock'></i> Повторете паролата</span>
				
					<button id='reg-button'>Регистрирай се!</button>			
					<button id='log-button'>Вход</button>				
				</section>";
			} else {
				echo "<div class='uzer-profile-header'><div id='arrow-container'><i id='left-icon' class='fa fa-angle-double-left'></i></div><div id='click-redirect'><img id='profile-image' src='/rei-assets/platige-image-reel-2016.jpg'><span id='user-profile-name'>".User::session("uzer", "log_name")."</span></div></div>";
				
			}
		?>
				
		</div>		

	</div>

</body>

<script type='text/javascript' src="http://code.jquery.com/jquery-2.0.3.min.js"></script>
<script type='text/javascript' src="/rei-assets/js/bootstrap.min.js"></script>
<script type='text/javascript' src='/rei-assets/js/home-main.js'></script>

<script type='text/javascript'>
if(window.navigator.cookieEnabled == false){
	$('#inner-cookie-notifier-container').css('display', 'block');
} else {
	if(getDaCookie("uname") !== undefined && getDaCookie("pass") !== undefined){
		if($('.login-form-section')[0]){
			$('input[name=username]').val(getDaCookie("uname"));
			$('input[name=password]').val(getDaCookie("pass"));	
			$('#remember-me').prop("checked", true);
		}
	} else {
		if($('.login-form-section')[0]){
			$('input[name=username]').val("");
			$('input[name=password]').val("");	
			$('#remember-me').prop("checked", false);	
		}
	}
}


function getDaCookie(cookieName){
	var name = cookieName + "=";
	var decodedCookie = decodeURIComponent(document.cookie);
	var ca = decodedCookie.split(';');
	for(var i = 0; i < ca.length; i++){
		var c = ca[i];
		while(c.charAt(0) == ' '){
			c = c.substring(1);
		}
		if(c.indexOf(cookieName) == 0){
			return c.split('=')[1];
		}
	}
}

if($('#sent-email-notifier')){
	$('#sent-email-notifier').delay(2500).fadeOut('slow');
}

if($('.uzer-profile-header')){
		$.ajax({url: "/upi", method: "post", async: true, data: "id=<?php echo intval(User::session("uzer", "id")); ?>", success: function(result){$('#profile-image').attr('src', result)}});
}
$('#click-redirect').click(function(){$.ajax({url: "/uss",method: "post",async: true,success: function(resultas){$(location).attr("href", "/user/"+resultas.split(',')[1]);}});});
let theUser = $('#user-profile-name').html(); let newUser = ""; if(theUser.length >= 12){newUser = theUser.replace(theUser.substring(10), "...");$('#user-profile-name').html(newUser);}
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




