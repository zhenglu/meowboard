<?php
/**
 * Dynamic Spotlight
 * This plugin will allow admins to modify the spotlight on the fly,
 * based on the original elgg spotlight file.
 * @package whimsyHood
 * @subpackage spotlight
 * @author David A Graham
 * @link http://www.davidallengraham.net/
 *
 */
?>
<div id="layout_spotlight">
<div id="wrapper_spotlight">
<div class="collapsable_box no_space_after">
	<div class="collapsable_box_header">
		<h1><?php echo elgg_echo("spotlight"); ?></h1>
		
	<?php 
echo get_plugin_setting($dynText, $dynmSpot = "");
	 ?>

</div>
</div>
	
</div><!-- /#wrapper_spotlight -->
</div><!-- /#layout_spotlight -->
