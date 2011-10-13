<?php
/**
 * Elgg standard tools drop down
 * This will be populated depending on the plugins active - only plugin navigation will appear here
 * extend by Dragan Jovanovic with tools submenu
 * @package Elgg
 * @subpackage Core
 *
 */

$menu = get_register('menu');

//var_export($menu);

if (is_array($menu) && sizeof($menu) > 0) {
	$alphamenu = array();
	foreach($menu as $item) {
		$alphamenu[$item->name] = $item;
	}
	ksort($alphamenu);

?>
	<li ><a class="drop" href="#"><?php echo(elgg_echo('tools')); ?></a>
        <div class="dropdown_1column"><!-- Begin 1columns container -->
         <div class="col_1">
            <ul class="greybox">
		<?php
		foreach($alphamenu as $item) {
			echo "<li><a href=\"{$item->value}\">" . $item->name . "</a></li>";			
		}

	?>
            </ul>
            </div>
        </div>
	</li>
	<?php
	$subtoolstitle = get_context();
	$submenu = get_submenu();
	if (!empty($submenu)) { 
	?>
	<li ><a class="drop" href="#"><?php echo $subtoolstitle; ?></a>
        <div class="dropdown_1column"><!-- Begin 1columns container -->
         <div class="col_1">
            <ul>
		<?php  
		
			echo $submenu ;
		?>
			</ul>
            </div>
        </div>
	</li>
	
<?php
}
}