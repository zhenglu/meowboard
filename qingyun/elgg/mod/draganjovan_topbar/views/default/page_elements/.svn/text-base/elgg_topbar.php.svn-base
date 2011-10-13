<?php

	/**
	 * Elgg draganjovan_topbar plugin
	 * 
	 * @author Dragan jovanovic
	 * @website www.draganjovan.info
	 */
?>

<?php if (isloggedin()) { ?>
<div id="draganjovan_topbar">
    <span><a href="<?php echo $vars['url']; ?>pg/dashboard/" ><img src="<?php echo $vars['url']; ?>_graphics/elgg_toolbar_logo.gif" alt="toolbar_logo graphic" /></a></span>
    
		<span class="draganjovan_topbar_left"><a <?php echo get_plugin_setting('linktarget','draganjovan_topbar');?> href="<?php echo get_plugin_setting('linkurl','draganjovan_topbar');?>"><?php echo get_plugin_setting('linktext','draganjovan_topbar');?></a></span>

		<span class="draganjovan_topbar_left user_mini_avatar"><a href="<?php echo get_loggedin_user()->getURL(); ?>"><img class="user_mini_avatar" src="<?php echo get_loggedin_user()->getIcon('topbar'); ?>" alt="User avatar" /></a></span>
       
		<span class="draganjovan_topbar_left"><?php echo elgg_view('elgg_topbar/extend', $vars); ?></span>
            <div class="draganjovan_topbar_right">
		<?php echo elgg_view('draganjovan_topbar/searchbox'); ?>
    </div>
          
	<ul>

			<?php echo elgg_view("navigation/topbar_tools"); ?>
			
				
               
		<li class="draganjovan_topbar_right"><a href="#" class="drop"><?php echo elgg_echo('draganjovan:account'); ?></a>
			
			
				<div class="dropdown_1column align_right">
				<div class="col_1">
                                    <div class="imgborder"><img  src="<?php echo get_loggedin_user()->getIcon('medium'); ?>" alt="User avatar" /></div>
					<ul>

                                            <li><a href="<?php echo $vars['url']; ?>pg/settings/" class="usersettings"><?php echo elgg_echo('settings'); ?></a></li>
							
			<?php if ($vars['user']->isAdmin()) { ?>
                                            <li><a href="<?php echo $vars['url']; ?>pg/admin/" class="usersettings"><?php echo elgg_echo("admin"); ?></a></li>
		
				<?php } ?>
							<li><?php echo elgg_view('output/url', array('href' => "{$vars['url']}action/logout", 'text' => elgg_echo('logout'), 'is_action' => TRUE)); ?></li>	
					</ul>
				</div>
				</div>
		</li>			
	</ul>
	

	
</div>
<div class="clearfloat"></div>
<?php
	}
