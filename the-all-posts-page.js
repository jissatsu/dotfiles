(function(){
	
	function $(e){
		if((/(\.[a-zA-Z0-9\-\_]*)/).exec(e)){
			return document.getElementsByClassName(e.substring(1));
		} 
		else if((/(\#[a-zA-Z0-9\-\_]*)/).exec(e)){
			return document.getElementById(e.substring(1));	
		}
	}
	
	let mainWidth = window.getComputedStyle($('.main-posts-container')[0], null).getPropertyValue('width');
	if(screen.width <= 1520){
		$('#inner-first-container').style.width = (mainWidth.split('px')[0] / 1.7) + 'px';
	} else {	
		$('#inner-first-container').style.width = (mainWidth.split('px')[0] / 1.5) + 'px';
	}
	
	for(let i = 0 ; i < $('.inner-posts-container').length ; i++){
		if($('.inner-posts-container')[i].children.length <= 6){
			$('.inner-posts-container')[i].style.height = 30 + '%';
			for(var children = 0 ; children < $('.inner-posts-container')[i].children.length; children++){
				$('.inner-posts-container')[i].children[children].style.height = 60 + '%';
			}
		}
	}

	for(let i = 2 ; i < $('.inner-posts-container').length ; i++){
		$('.inner-posts-container')[i].style.display = 'none';
		$('.section-num')[i].style.display = 'none';
	}
	
	document.body.onscroll = function(){
		let docHeight = document.documentElement.scrollHeight || document.body.scrollHeight;	
		let windowHeight = window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight;
		let scrolled = window.pageYOffset || document.documentElement.scrollTop || document.body.parentNode.scrollTop;
		
		let percent = Math.ceil(scrolled / (docHeight - windowHeight) * 100);	
		
		setInterval(function(){
			for(var i = 2; i < $('.inner-posts-container').length ; i++){
				if(percent == 100){
					$('.inner-posts-container')[i].style.display = 'block';
					$('.section-num')[i].style.display = 'flex';
				}
			}
		}, 500);
	}	

})();
