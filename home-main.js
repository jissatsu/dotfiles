(function(){
	
	function Ajaxer(url, data){
		$.ajax({
			url: url,
			method: 'post',
			async: true,
			data: "postData="+data,
			success: function(response){
				if(response == "l"){
					if($('#remember-me').is(':checked')){
						createDaCookie("uname", $('input[name=username]').val(), 1);	
						createDaCookie("pass", $('input[name=password]').val(), 1);	
						window.location.href = "/";
					} else {
						createDaCookie("uname", "", -1);	
						createDaCookie("pass", "", -1);	
						window.location.href = "/";
					}
				} else if(response == "r"){
					window.location.href = "/";
				}
				$result = response.split(',');
				for(var i = 0 ; i < $result.length ; i++){
					if((/(user=)/).exec($result[i])){
						$('#user-reg-err').html($result[i].substring(5)) && $('#user-reg-err').delay(500).fadeIn('slow') && $('#user-reg-err').delay(2000).fadeOut('slow');
					} 
					else if((/(email=)/).exec($result[i])){
						$('#email-reg-err').html($result[i].substring(6)) && $('#email-reg-err').delay(500).fadeIn('slow') && $('#email-reg-err').delay(2000).fadeOut('slow');
					} 
					else if((/(pass1=)/).exec($result[i])){
						$('#pass1-reg-err').html($result[i].substring(6)) && $('#pass1-reg-err').delay(500).fadeIn('slow') && $('#pass1-reg-err').delay(2000).fadeOut('slow');	
					}
					else if((/(pass2=)/).exec($result[i])){
						$('#pass2-reg-err').html($result[i].substring(6)) && $('#pass2-reg-err').delay(500).fadeIn('slow') && $('#pass2-reg-err').delay(2000).fadeOut('slow');	
					}
					else if((/(no_match)/).exec($result[i])){
						$('#pass2-reg-err').html('Паролите не съвпадат!') && $('#pass2-reg-err').delay(500).fadeIn('slow') && $('#pass2-reg-err').delay(2000).fadeOut('slow');	
					}
					else if((/(exist=)/).exec($result[i])){
						$('#user-reg-err').html($result[i].substring(6)) && $('#user-reg-err').delay(500).fadeIn('slow') && $('#user-reg-err').delay(2000).fadeOut('slow');	
					}
					else if((/logUser=/).exec($result[i])){
						$('#user-log-err').html($result[i].substring(8)) && $('#user-log-err').delay(500).fadeIn('slow') && $('#user-log-err').delay(2000).fadeOut('slow');
					}
					else if((/logPass=/).exec($result[i])){
						$('#pass-log-err').html($result[i].substring(8)) && $('#pass-log-err').delay(500).fadeIn('slow') && $('#pass-log-err').delay(2000).fadeOut('slow');
					}
				}
			}
		});		
	}

	$(document).ready(function(){
		
		clock();
		stop_loader_animation();
		
		$('#the-carousel').carousel({interval: 5000});

		$('#the-carousel').delay(1000).fadeIn('slow');
		$('.item1').click(function(){
			$('#the-carousel').carousel(0);
		});
		$('.item2').click(function(){
			$('#the-carousel').carousel(1);
		});
		$('.item3').click(function(){
			$('#the-carousel').carousel(2);
		});
		$('.item4').click(function(){
			$('#the-carousel').carousel(3);
	});
	
	
	
	$('body').click(function(e){
		if(e.target.id == 'register-button'){
			if($('.login-form-section').css('display') !== 'none'){
				$('.login-form-section').css('display', 'none');
				$('#register-form').css('display', 'flex');
			}			
		}
		else if(e.target.id == 'log-button'){
			$('#register-form').css('display', 'none');
			$('.login-form-section').removeAttr('id');
			$('.login-form-section').css({'width':'29%', 'margin-left':'35.5%'});
			$('.login-form-section').css('display', 'flex');
		}

		if(e.target.id == 'reg-button'){
			$date = new Date();

			$date_day = ( $date.getDate() < 10 ) ? "0"+$date.getDate() : $date.getDate();
			$date_month = ( $date.getMonth() < 10 ) ? "0"+$date.getMonth() : $date.getMonth();
			$date_year = $date.getFullYear();
			$date_hour = ( $date.getHours() == 0 ) ? "0"+$date.getHours() : $date.getHours();
			$date_min = ( $date.getMinutes() < 10 ) ? "0"+$date.getMinutes() : $date.getMinutes();

			$fullDate = $date_day + "/" + $date_month + "/" + $date_year + "-" + $date_hour + ":" + $date_min; 

			$username = $('input[name=field0]').val();
			$email = $('input[name=field1]').val();
			$pass1 = $('input[name=field2]').val();
			$pass2 = $('input[name=field3]').val();
			dataToSend = JSON.stringify({"username":$username, "email":$email, "pass1":$pass1, "pass2":$pass2, "date":$fullDate});
			Ajaxer('/user/register', dataToSend);
		}

		if(e.target.id == 'login-button'){
			$userName = $('input[name=username]').val();
			$userPass = $('input[name=password]').val();
			$data = JSON.stringify({"uname":$userName, "pass":$userPass});
			Ajaxer('/user/login', $data);
		}

		else if( (e.target.id == 'arrow-container' || e.target.id == 'left-icon') && !$('#left-icon').hasClass('fa fa-angle-double-right')){
			$('.uzer-profile-header').attr('id', 'to-left-slide');
			$('#left-icon').removeAttr('class') && $('#left-icon').attr('class', 'fa fa-angle-double-right');	
		}
	
		else if( (e.target.id == 'arrow-container' || e.target.id == 'left-icon') && $('#left-icon').hasClass('fa fa-angle-double-right')){
			$('.uzer-profile-header').removeAttr('id');
			$('.uzer-profile-header').attr('id', 'to-right-slide');
			$('#left-icon').removeAttr('class') && $('#left-icon').attr('class', 'fa fa-angle-double-left');
		}

	});


	$('.inner-login-form-container').delay(3000).fadeIn('slow');

	$('.second-main-wrapper-container').append('<span id="user-reg-err"></span>');
	$('.second-main-wrapper-container').append('<span id="email-reg-err"></span>');
	$('.second-main-wrapper-container').append('<span id="pass1-reg-err"></span>');
	$('.second-main-wrapper-container').append('<span id="pass2-reg-err"></span>');
	$('.second-main-wrapper-container').append('<span id="user-log-err"></span>');
	$('.second-main-wrapper-container').append('<span id="pass-log-err"></span>');


	$('#close-window').click(function(){
		if($('#inner-cookie-notifier-container').css('display') == 'none'){
			$('#cookie-notifier').css('display', 'none');
		} else {
			$('#inner-cookie-notifier-container').css('display', 'none');
		}
	});

});


function createDaCookie(name, value, exdays){
	var date = new Date();
	date.setTime(date.getTime() + (exdays*24*60*60*1000));
	var expires = "expires="+date.toUTCString();
	document.cookie = name + "=" + value + ";" + expires + ";path=/";
}


function clock(){
	var clock = setInterval(function(){
		var time = new Date();
		var icon = '';
		
		var hours = ( time.getHours() == 0 ) ? "0"+time.getHours() : time.getHours();

		var mins = ( time.getMinutes() < 10 ) ? "0"+time.getMinutes() : time.getMinutes();
		if(hours >= 19 || hours <= 5){
			icon = 'moon';
		} else {
			icon = 'sun';
		}

		$('#inner-cookie-notifier-timer').find('span').html(' <i id="day-n-night" class="fa fa-'+icon+'-o" style="color: #686899;"></i> ' + hours + ":" + mins);
	}, 1000);
}


function stop_loader_animation(){
	var count = 0;
	var stop = setInterval(function(){		
		if(count <= 1){
			$('.main-wrapper-container').css('background-image', 'none');	
			count++;
		} else {
			clearInterval();
		}
	}, 4000);
}

	
function createRegisterForm(){
	var section = document.getElementById('register-form');
	var registerUpper = document.createElement('div');
	var registerFormText = document.createElement('span');
	
	if(section){

		registerFormText.setAttribute('id', 'register-form-text');
		registerFormText.appendChild(document.createTextNode('Регистрация'));
	
		registerUpper.setAttribute('id', 'register-form-upper');
		registerUpper.appendChild(registerFormText);
	
		section.appendChild(registerUpper);
		
		for(var elements = 0; elements < 4; elements++){
			var element = document.createElement('input');
			element.setAttribute('id', 'input'+elements);
			element.setAttribute('type', 'text');
			element.setAttribute('name', 'field'+elements);
			if(elements == 1){
				element.setAttribute('type', 'email');
			} else if(elements == 2 || elements == 3){
				element.setAttribute('type', 'password');
			}
			section.appendChild(element);
		}
		
	}
}

createRegisterForm();

})();
