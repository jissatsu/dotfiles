function Voter() {}
Voter.prototype.init = function( postData, elements ){
	this.vote_elements_data      = elements;                                                 // a list of size 2 [ first element contains info about the vote elements ; second element is the reset vote element ]
	this.vote_elements_container = this.vote_elements_data[3]['vote-elements-container'];
	this.autoSend                = 'disabled';                                               // used to check the score made every time the page is visited ( if enabled an empty string is sent to server )
	this.voteResultElement       = this.vote_elements_data[2]['id'];                         // id of element to append vote result to without reloading the page
	this.postType                = postData['post-type'];                                    // the type of the post [thread or discussion]
	this.postName                = postData['post-name'];                                    // the name of the post 
	this.data                    = {};                                                       // data to be sent to the server
	this.aResponse               = '';                                                       // the response string from server
	this.data_name_from          = this.vote_elements_data[0]['data-name']['from'];          // change data-name attribute's value from this 
	this.data_name_to            = this.vote_elements_data[0]['data-name']['to'];            // to this
	this.class_name_from         = this.vote_elements_data[0]['class']['from'];              // change class attribute's value from this 
	this.class_name_to           = this.vote_elements_data[0]['class']['to'];                // to this
	this.IDS                     = this.vote_elements_data[0]['id']['name'];                 // change id attribute's value from this
	this.ID_NUM_RANGE            = this.vote_elements_data[0]['id']['range'].split('-');     // to this
	this.resetVote               = this.vote_elements_data[1]['id'];                         // this is the reset vote element's id ( when clicked on, this element deletes the user's vote on the current post )
	this.starColors              = [ this.vote_elements_data[0]['star-colors']['from'], this.vote_elements_data[0]['star-colors']['to'] ];
	this.points                  = 0;
	this.spinWait                = false;
}



Voter.prototype.setDataURL = function( url )
{
	this.url = url;
}



Voter.prototype.elementTransitionTo = function( el, from, to )
{
	var isClass = 0;
	if( from.substring(0, 1) == '.' && to.substring(0, 1) == '.' )
	{
		isClass = 1;
	}

	if( isClass )
	{
		el.attr( 'class', 'fa '+to.substring(1) );
	}
	else{
		el.attr( 'data-name', to );
	}
}



Voter.prototype.sendReq = function( http, method, url, data, transition, startSpin )
{
	http.onreadystatechange = function(){
		if( this.readyState == 4 && this.status == 200 )
		{
			if( (/v=/).exec( this.responseText ) )
			{
				for( var i = parent.voter.ID_NUM_RANGE[0] ; i <= parseInt( this.responseText.substring(2) ) ; i++ )
				{
					transition( $(parent.voter.IDS+i), parent.voter.class_name_from, parent.voter.class_name_to );
				}
			}

			else if( (/g=/).exec( this.responseText ) )
			{
				$(parent.voter.voteResultElement).html( this.responseText.substring(2) );
			}
			
			if( parent.voter.spinWait ) { parent.voter.spinAct( 0 ); }
		}

		else if( this.readyState == 3 )
		{
			if( parent.voter.spinWait ) { parent.voter.spinAct( 1 ); }
		}
	};
	
	http.open( method, url, true );
	http.setRequestHeader( "Content-Type", "application/x-www-form-urlencoded" );
	http.send( "t-d="+data );
}



Voter.prototype.die = function( message )
{
	throw new Error( '[ ERROR ] - ' + message );
}



Voter.prototype.ajaxInit = function( reqType )
{
	var http;
	if( window.XMLHttpRequest )
	{
		http = new XMLHttpRequest();
	}
	else{
		http = new ActiveXObject( 'Microsoft.XMLHTTP' );
	}
	return http;
}



Voter.prototype.spinAct = function( act )
{
	if( this.vote_elements_container != '' )
	{
		if( act == 1 )
		{
			$(this.vote_elements_container).append( "<i id='vote-spin' class='fa fa-spinner fa-spin fa-fw' style='position: absolute;color: #b6c1d3; left: 10%;margin-top: 0.5%;'></i>" );
		}
		else{
			$('#vote-spin').remove();
		}
	}
}



Voter.prototype.Action = function()
{	
	var http = this.ajaxInit();


	if( this.url == undefined || this.data_name_from == '' || this.data_name_to == '' || this.class_name_from == '' 
                           	  || this.class_name_to == ''  || this.IDS == ''          || this.ID_NUM_RANGE == '' || this.resetVote == '' )
	{
		this.die( 'Missing required parameters!' );
	}


	if( this.autoSend == 'enabled' )
	{	
		this.data = JSON.stringify({ 'tp': '', 'tn': this.postName });
		this.sendReq( http, "POST", this.url, this.data, this.elementTransitionTo );
	}
	


	$(this.resetVote).click( function(){
		parent.voter.points = 0;
		parent.voter.data   = JSON.stringify({ 'tp': parent.voter.points, 'tn': parent.voter.postName });
		
		for( var i = parent.voter.ID_NUM_RANGE[0] ; i <= parent.voter.ID_NUM_RANGE[1] ; i++ )
		{
			parent.voter.elementTransitionTo( $(parent.voter.IDS+i), parent.voter.data_name_from, parent.voter.data_name_to );
			parent.voter.elementTransitionTo( $(parent.voter.IDS+i), parent.voter.class_name_to, parent.voter.class_name_from );
		}
		
		parent.voter.sendReq( http, "POST", parent.voter.url, parent.voter.data );
	});


	
	$.each( $(this.class_name_from), function( i, v ){
		$(v).click( function(){
			for( var j = parent.voter.ID_NUM_RANGE[0] ; j <= parent.voter.ID_NUM_RANGE[1] ; j++ )
			{
				if( $(parent.voter.IDS+j).attr( 'data-name' ) == parent.voter.data_name_from )
				{
					parent.voter.elementTransitionTo( $(parent.voter.IDS+j), parent.voter.data_name_from, parent.voter.data_name_to );
					parent.voter.elementTransitionTo( $(parent.voter.IDS+j), parent.voter.class_name_to, parent.voter.class_name_from );
					parent.voter.points = Number( $(parent.voter.IDS+(i+1)).attr( 'data-value' ) );
				}
			}

			for( var k = parent.voter.ID_NUM_RANGE[0] ; k <= (i+1) ; k++ )
			{
				parent.voter.elementTransitionTo( $(parent.voter.IDS+k), parent.voter.data_name_to, parent.voter.data_name_from );
				parent.voter.elementTransitionTo( $(parent.voter.IDS+k), parent.voter.class_name_from, parent.voter.class_name_to );
				parent.voter.points = Number( $(parent.voter.IDS+(i+1)).attr( 'data-value' ) );
			}
			parent.voter.data = JSON.stringify({ 'tp': parseInt( parent.voter.points ), 'tn': parent.voter.postName });
			parent.voter.sendReq( http, "POST", parent.voter.url, parent.voter.data );
		});
	});
}

