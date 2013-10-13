/**
 * This file is part of OpenMediaVault.
 *
 * @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
 * @author    Volker Theile <volker.theile@openmediavault.org>
 * @copyright Copyright (c) 2009-2013 Volker Theile
 *
 * OpenMediaVault is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * OpenMediaVault is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenMediaVault. If not, see <http://www.gnu.org/licenses/>.
 */
// require("js/omv/WorkspaceManager.js")
// require("js/omv/workspace/panel/Panel.js")

/**
 * @class OMV.module.admin.service.links.Omvlinks
 * @derived OMV.workspace.panel.Panel
 */
Ext.define("OMV.module.admin.service.links.Omvlinks", {
	extend: "OMV.workspace.panel.Panel",

	initComponent: function() {
		var me = this;
		me.html = "<form style='overflow: auto; height: 100%;'>";
		me.html += me.createBox(
			"<b>OMVforums</b><br/><a href='http://forums.openmediavault.org' target='_blank'>http://forums.openmediavault.org</a><br/><br/>" +
			"<b>OMVsourceforge</b><br/><a href='http://sourceforge.net/projects/openmediavault/' target='_blank'>http://sourceforge.net/projects/openmediavault/</a><br/><br/>" +
			"<b>OMVwiki</b><br/><a href='http://wiki.openmediavault.org/' target='_blank'>http://wiki.openmediavault.org/</a><br/><br/>" +
			"<b>OMVfacebook</b><br/><a href='https://www.facebook.com/pages/OpenMediaVault/107543672670199' target='_blank'>https://www.facebook.com/pages/OpenMediaVault/107543672670199</a><br/><br/>" +
			"<b>OMVtwitter</b><br/><a href='https://twitter.com/OpenMediaVault' target='_blank'>https://twitter.com/OpenMediaVault</a><br/><br/>" +
			"<b>OMVblog</b><br/><a href='http://blog.openmediavault.org/' target='_blank'>http://blog.openmediavault.org/</a><br/><br/>" +
			"<b>GitHub</b><br/><a href='https://github.com/' target='_blank'>https://github.com/</a><br/><br/>" +
			"<b>Documentation</b><br/><a href='http://docs.openmediavault.org' target='_blank'>http://docs.openmediavault.org</a><br/><br/>" +
			"<b>Ext JS 4.2</b><br/><a href='http://docs.sencha.com/extjs/4.2.2/#' target='_blank'>http://docs.sencha.com/extjs/4.2.2/#</a><br/><br/>" +
			"<b>JSFiddle</b><br/><a href='http://jsfiddle.net/' target='_blank'>http://jsfiddle.net/</a><br/><br/>" +
			"<b>GitHub</b><br/><a href='https://github.com/' target='_blank'>https://github.com/</a><br/><br/>" +
			"<b>Subversion</b><br/><a href='http://subversion.apache.org/' target='_blank'>http://subversion.apache.org/</a><br/><br/>" +
			"<b>Bugtracker</b><br/><a href='http://bugtracker.openmediavault.org' target='_blank'>http://bugtracker.openmediavault.org</a><br/><br/>" +
			"<b>Aptana</b><br/><a href='http://www.aptana.com/' target='_blank'>http://www.aptana.com/</a><br/><br/>" +
			"<b>Tortoise SVN</b><br/><a href='http://tortoisesvn.net/' target='_blank'>http://tortoisesvn.net/</a><br/><br/>" +	
			"<b>Notepad++</b><br/><a href='http://notepad-plus-plus.org/' target='_blank'>http://notepad-plus-plus.org/</a><br/><br/>" +
			"<b>Putty</b><br/><a href='http://www.putty.org/' target='_blank'>http://www.putty.org/</a><br/><br/>" +
			"<b>SystemRescueCD</b><br/><a href='http://www.sysresccd.org/' target='_blank'>https://www.sysresccd.org/</a><br/><br/>" +
			"<b>Parted Magic</b><br/><a href='http://partedmagic.com/' target='_blank'>http://partedmagic.com/</a><br/>");
			"<b>Clonezilla</b><br/><a href='http://clonezilla.org/' target='_blank'>http://clonezilla.org/</a><br/>");
			"<b>GParted</b><br/><a href='http://gparted.sourceforge.net/index.php' target='_blank'>http://gparted.sourceforge.net/index.php</a><br/>");
		me.html += "</form>";
		me.callParent(arguments);
	},

	createBox: function(msg) {
		return [ '<div class="x-box-aboutbox">', msg, '</div>' ].join('');
	}
});

OMV.WorkspaceManager.registerPanel({
	id: "omvlinks",
	path: "/service/links",
	text: _("Common URLs"),
	position: 20,
	className: "OMV.module.admin.service.links.Omvlinks"
});
