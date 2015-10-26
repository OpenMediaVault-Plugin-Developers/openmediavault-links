/**
 * This file is part of OpenMediaVault.
 *
 * @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
 * @author    Volker Theile <volker.theile@openmediavault.org>
 * @copyright Copyright (c) 2009-2015 Volker Theile
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
// require("js/omv/workspace/dashboard/View.js")
// require("js/omv/data/Store.js")
// require("js/omv/data/Model.js")
// require("js/omv/data/proxy/Rpc.js")
// require("js/omv/workspace/grid/Panel.js")
// require("js/omv/util/Format.js")

/**
 * @class OMV.module.admin.dashboard.view.Links
 * @derived OMV.workspace.dashboard.View
 */
Ext.define("OMV.module.admin.dashboard.view.Links", {
	extend: "OMV.workspace.dashboard.View",
	alias: "widget.module.admin.dashboard.view.Links",
	requires: [
		"OMV.workspace.grid.Panel",
		"OMV.data.Store",
		"OMV.data.Model",
		"OMV.data.proxy.Rpc",
		"OMV.util.Format"
	],

	height: 150,
	refreshInterval: 60000,

initComponent : function() {
        var me = this;
        Ext.apply(me,{
			items: [ me.gp = Ext.create("OMV.workspace.grid.Panel", {
				disableLoadMaskOnLoad: true,
				hideTopToolbar: true,
				hidePagingToolbar: true,
				disableSelection: true,
				stateful: true,
				stateId: "a982a76d-6804-4632-b31b-8b48c0ea6dde",
				columns           : [{
        text      : _("Name"),
        sortable  : true,
        dataIndex : "name",
        stateId   : "name"
    },{
        text      : _("Group"),
        sortable  : true,
        dataIndex : "group",
        stateId   : "group"
    },{
        text      : _("URL"),
        sortable  : true,
        dataIndex : "url",
        flex      : 1,
        stateId   : "url",
        renderer  : function(value) {
            var link = value.replace("\" + location.hostname + \"", location.hostname);
            return "<a href=\"" + link + "\" target=\"_blank\">" + link + "</a>";
        }
    }],
				viewConfig: {
					trackOver: false
				},
				store: Ext.create("OMV.data.Store", {
                autoLoad   : true,
                groupField : "group",
                model      : OMV.data.Model.createImplicit({
                    idProperty  : "uuid",
                    fields      : [
                        { name : "uuid", type : "string" },
                        { name : "name", type : "string" },
                        { name : "group", type : "string" },
                        { name : "url", type : "string" }
                    ]
                }),
                proxy : {
                    type    : "rpc",
                    rpcData : {
                        service : "Links",
                        method  : "getLinks"
                    }
                },
                sorters : [{
                    direction : "ASC",
                    property  : "name"
                },{
                    direction : "ASC",
                    property  : "group"
                }]
            })
			}) ]
		});
        me.callParent(arguments);
    },

	doRefresh: function() {
		var me = this;
		me.gp.doReload();
	}
});
