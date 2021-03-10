<?php
   //game=doteater+version=0.4.5.3+nick=andreboyce+IP=127.0.0.1+port=7654
   //$_REQUEST
   //$_SERVER['REQUEST_URI']
   echo $_SERVER['REQUEST_URI'];
   function eraseGame( $filename = "games.txt" )
   {
       file_put_contents( $filename, '' );
   }
   eraseGame();   
?>
