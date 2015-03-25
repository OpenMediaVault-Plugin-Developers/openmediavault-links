<?php
/**
 * @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
 * @author    Volker Theile <volker.theile@openmediavault.org>
 * @copyright Copyright (c) 2009-2013 Volker Theile
 * @copyright Copyright (c) 2013-2015 OpenMediaVault Plugin Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
require_once("openmediavault/object.inc");
require_once("openmediavault/config.inc");
require_once("openmediavault/error.inc");
require_once("openmediavault/util.inc");
require_once("openmediavault/rpcservice.inc");
require_once("openmediavault/notify.inc");

class OMVRpcServiceLinks extends OMVRpcServiceAbstract {
    /**
     * Get the RPC service name.
     */
    public function getName() {
        return "Links";
    }

    private function getPluginName()
    {
        return strtolower($this->getName());
    }

    private function getXPath()
    {
        return sprintf("//services/%s", $this->getPluginName());
    }

    private function getXPathLink()
    {
        return sprintf("%s/link", $this->getXPath());
    }

    private function getEventMessagePath()
    {
        return sprintf("org.openmediavault.services.%s.link", $this->getPluginName());
    }

    /**
     * Initialize the RPC service.
     */
    public function initialize()
    {
        $this->registerMethod("getLinks");
        $this->registerMethod("getLink");
        $this->registerMethod("setLink");
        $this->registerMethod("deleteLink");
        $this->registerMethod("enumeratePresets");
    }

    /**
     * Get links
     */
    public function getLinks($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext(
            $context,
            array( "role" => OMV_ROLE_ADMINISTRATOR )
        );

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, '{
            "type"       : "object",
            "properties" : {
                "start"     : { "type" : "integer" },
                "limit"     : { '.$GLOBALS['OMV_JSONSCHEMA_COUNTFIELD'].' },
                "sortfield" : { '.$GLOBALS['OMV_JSONSCHEMA_SORTFIELD'].' },
                "sortdir"   : { '.$GLOBALS['OMV_JSONSCHEMA_SORTDIR'].' }
            }
        }');

        // Get configuration data.
        $objects = $xmlConfig->getList( $this->getXPathLink() );
        if (is_null($objects)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getXPathLink()
            );
        }

        // Filter the result.
        return $this->applyFilter($objects, $params['start'], $params['limit'],
            $params['sortfield'], $params['sortdir']);
    }

    /**
     * Get a link configuration object.
     */
    public function getLink($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext(
            $context,
            array("role" => OMV_ROLE_ADMINISTRATOR)
        );

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, '{
            "type"       : "object",
            "properties" : {
                "uuid" : { '.$GLOBALS['OMV_JSONSCHEMA_UUID'].' }
            }
        }');

        // Get the configuration object.
        $xpath = sprintf("%s[uuid='%s']", $this->getXPathLink(), $params['uuid']);
        $object = $xmlConfig->get($xpath);
        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $xpath
            );
        }

        // Modify the result object.
        $object['createtab'] = boolval($object['createtab']);
        
        return $object;
    }

    /**
     * Set (add/update) a link.
     */
    public function setLink($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext(
            $context,
            array( "role" => OMV_ROLE_ADMINISTRATOR )
        );

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, '{
            "type"       : "object",
            "properties" : {
                "uuid"      : { '.$GLOBALS['OMV_JSONSCHEMA_UUID_UNDEFINED'].' },
                "name"      : { "type" : "string" },
                "group"     : { "type" : "string" },
                "url"       : { "type" : "string" },
                "createtab" : { "type" : "boolean" }
            }
        }');

        // Prepare configuration data.
        $object = array(
            "uuid"      => ($params['uuid'] == $GLOBALS['OMV_UUID_UNDEFINED']) ? OMVUtil::uuid() : $params['uuid'],
            "name"      => $params['name'],
            "group"     => $params['group'],
            "url"       => $params['url'],
            "createtab" => array_boolval($params, 'createtab'),
        );

        // Set the configuration data.
        $success = FALSE;
        if($params['uuid'] == $GLOBALS['OMV_UUID_UNDEFINED']) {
            // Append object to configuration.
            $success = $xmlConfig->set($this->getXPath(), array("link" => $object));
        } else {
            // Update existing configuration object.
            $xpath = sprintf("%s[uuid='%s']", $this->getXPathLink(), $params['uuid']);
            $success = $xmlConfig->replace($xpath, $object);
        }
        if (FALSE === $success) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED
            );
        }

        // Notify configuration changes.
        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(
            ($params['uuid'] == $GLOBALS['OMV_UUID_UNDEFINED']) ? OMV_NOTIFY_CREATE : OMV_NOTIFY_MODIFY,
            $this->getEventMessagePath(),
            $object
        );

        // Return the configuration object.
        return $object;
    }

    /**
     * Delete a link configuration object.
     */
    public function deleteLink($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext(
            $context,
            array( "role" => OMV_ROLE_ADMINISTRATOR )
        );

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, '{
            "type"       : "object",
            "properties" : {
                "uuid" : { '.$GLOBALS['OMV_JSONSCHEMA_UUID'].' }
            }
        }');

        // Delete the configuration object.
        $xpath = sprintf("%s[uuid='%s']", $this->getXPathLink(), $params['uuid']);
        return $this->deleteConfigObjectByPath(
            $xpath,
            $this->getEventMessagePath()
        );
    }

    /**
     * Enumerate service presets.
     */
    public function enumeratePresets($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext(
            $context,
            array( "role" => OMV_ROLE_ADMINISTRATOR )
        );

        $presets = [];

        $presets[] = ["name" => "eXtplorer", "url" => "/extplorer/"];
        $presets[] = ["name" => "GateOne", "url" => "https://\" + location.hostname + \":8777/gateone/"];
        $presets[] = ["name" => "Media Browser", "url" => "http://\" + location.hostname + \":8096/mediabrowser/dashboard/dashboard.html"];
        $presets[] = ["name" => "MySql", "url" => "/mysql/"];
        $presets[] = ["name" => "Plexmediaserver", "url" => "http://\" + location.hostname + \":32400/manage"];
        $presets[] = ["name" => "PostgreSql", "url" => "/postgresql/"];
        $presets[] = ["name" => "Roundcube", "url" => "/webmail/"];
        $presets[] = ["name" => "Sabnzbd", "url" => "/sabnzbd/"];
        $presets[] = ["name" => "Sickbeard", "url" => "/sickbeard/"];
        $presets[] = ["name" => "Subsonic", "url" => "/subsonic/"];
        $presets[] = ["name" => "Shellinabox", "url" => "/shellinabox/"];
        $presets[] = ["name" => "Teamspeak3", "url" => "/ts3wi/"];
        $presets[] = ["name" => "VirtualBox", "url" => "/virtualbox/"];
        $presets[] = ["name" => "Wordpress", "url" => "/wordpress/"];

        $presetsWithPort = [
            "Calibre",
            "CouchPotato",
            "Deluge",
            "Headphones",
            "Mcmyadmin",
            "Nzbdrone",
            "Nzbget",
            "Pyload",
            "Sabnzbd",
            "Sickbeard",
            "Sonarr",
            "Subsonic"
        ];

        foreach ($presetsWithPort as $service) {
            $port = $this->getServicePort($service);

            // Skip service if no port exists.
            if (!$port) {
                continue;
            }

            $presets[] = [
                "name" => $service, 
                "url"  => "http://\" + location.hostname + \":$port"
            ];
        }

        return $presets;
    }

    private function getServicePort($service)
    {
        global $xmlConfig;

        // Get configuration object
        $xpath = sprintf("//services/%s", strtolower($service));
        $object = $xmlConfig->get($xpath);
        
        if (is_null($object)) {
            return null;
        } else {
            return $object['port'];
        }
    }
}

// Register the RPC service.
$rpcServiceMgr = &OMVRpcServiceMgr::getInstance();
$rpcServiceMgr->registerService(new OMVRpcServiceLinks());
