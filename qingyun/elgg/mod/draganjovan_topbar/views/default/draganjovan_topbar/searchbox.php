<?php

if (array_key_exists('value', $vars)) {
	$value = $vars['value'];
} elseif ($value = get_input('q', get_input('tag', NULL))) {
	$value = $value;
} else {
	$value = elgg_echo('search');
}

$value = stripslashes($value);

?>

<form class="searchform" action="<?php echo $vars['url']; ?>pg/search/" method="get">
	<input type="text" size="21" name="q" value="<?php echo $value; ?>" onclick="if (this.value=='<?php echo elgg_echo('search'); ?>') { this.value='' }" class="searchfield"  />
	<input type="submit" value="<?php echo elgg_echo('search:go'); ?>" class="searchbutton" />
</form>
