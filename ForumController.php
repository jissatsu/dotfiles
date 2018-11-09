<?php 

class ForumController
{	

	public $thread_name, $thread_by, $thread_date;
	
	
	public function add_new_thread( array $thread_data, string $table, object $db ) : bool
	{	
		$thread_table_name  = $thread_data['thread_name']."_t";
		$thread_name 		= $thread_data['thread_name'];
		$thread_by_uname	= $thread_data['thread_by_username'];
		$thread_by_id 		= $thread_data['thread_by_id'];
		$thread_c_date 		= $thread_data['thread_c_date'];
		
		$Q = array(
				"c_thread_r" => "INSERT INTO $table ( thread_name, thread_by_username, thread_by_id, thread_c_date ) VALUES ( ?, ?, ?, ? )",
				"c_thread_t" => "CREATE TABLE `$thread_table_name` (id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, 
											 	 				  discuss_name VARCHAR( 255 ), 
											  					  discuss_by_username VARCHAR(45), 
											  					  discuss_by_id VARCHAR(11), 
											  					  discuss_picture VARCHAR(255), 
																  discuss_rating VARCHAR(11),
																  discuss_c_date VARCHAR(20))");
		$s = $db->prepare( $Q['c_thread_r'] );
		$s->bind_param( 'ssis', $thread_name, 
							    $thread_by_uname, 
							    $thread_by_id,
								$thread_c_date
		);
		if( $s->execute() )
		{	
			if( $db->query( $Q['c_thread_t'] ) )
			{
				return true;
			}
		}
		return false;
	}


	public function add_new_post( array $post_data, array $on_thread_data, string $postType, object $db )
	{
		
	}


	public function count_data( string $table, object $db ) : int
	{
		$q = "SELECT COUNT(*) AS data_count FROM `$table`";
		$r = $db->query( $q );
		$d = @$r->fetch_assoc();
		$count = (int) $d['data_count'];
		
		return $count;
	}


	public function get_data( int $page_num, int $count_per_page, string $table, object $db )
	{		
		$q = "SELECT * FROM `$table` LIMIT ? OFFSET ?";
		$prefix = '';
		$cols = array();

		if( $table == "forum_threads" ) { $prefix = 'thread_'; }
		else { $prefix = 'discuss_'; }

		$data 		 = array();
		$count 		 = $this->count_data( $table, $db );
		$total_pages = (int) ceil( $count / $count_per_page );
		
		$limit	 = (int) $count_per_page;
		if( $page_num > 1 )
		{
			$offset  = $count_per_page * ( $page_num - 1 );
		} else{
			$offset = 0;
		}
		
		$data['pages_total'] = $total_pages;
		$data['count'] = $count;

		$get_data = $db->prepare( $q );
		$get_data->bind_param( 'ii', $limit, $offset );
		$get_data->execute();
		
		$r = @$get_data->get_result();
		while( $g = @$r->fetch_assoc() )
		{	
			$a = str_replace( array( "{xd|right}", "{xd|zzz}", "{xd|left}", "{xd|doub}", "{xd|sing}"), 
							  array( ">", "`", "<", "\"", "'" ),  
							  $g[$prefix.'name'] );

			$t_pic = ( is_null($g[$prefix.'picture']) ) ? "/rei-assets/platige-image-reel-2016.jpg" : htmlentities( "/rei-assets/forum/".$g[$prefix.'picture'], ENT_QUOTES, "UTF-8" ) ;

			array_push( $data, array( 
							"t_name" => htmlentities($a, ENT_QUOTES, "UTF-8"),
							"t_b_u"  => htmlentities( $g[$prefix.'by_username'], ENT_QUOTES, "UTF-8"),
							"t_b_i"  => (int) $g[$prefix.'by_id'],
							"t_pic"  => $t_pic,
							"t_rat"  => $g[$prefix.'rating'])
			);
		}
		
		
		return $data;
	}


	public function vote_thread( array $thread_data, int $points, object $db )
	{
		
	}
}

?>
