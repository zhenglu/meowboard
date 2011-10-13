<p>
	<?php echo elgg_echo('draganjovan:adminsettings'); ?>
	<ul><li><?php echo elgg_echo('draganjovan:admintext'); ?> <input type="text" value="<?php echo $vars['entity']->linktext; ?>" name="params[linktext]" /></li>
	<li><?php echo elgg_echo('draganjovan:adminurl'); ?> <input type="text" value="<?php echo $vars['entity']->linkurl; ?>" name="params[linkurl]" /></li>
        <li><?php echo elgg_echo('draganjovan:admintarget'); ?> <input type="text" value="<?php echo $vars['entity']->linktaarget; ?>" name="params[linktarget]" /></li>
	</ul>
</p>
