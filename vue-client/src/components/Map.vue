<template>
  <v-container class="pa-2" fluid>
    <v-row no-gutters>
      <v-col>
        <v-card height="100%">
          <div id="map">
            <Popup />
          </div>
        </v-card>
      </v-col>
      <v-col :md="2">
        <Positions
          :positions="startPositions"
          :gettingCenter="gettingCenter"
          @removePos="onRemovePos"
          @getCenter="getPathsFromDijkstra"
          @clearMap="onClearMap"
        />
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import axios from "axios";

// openlayers
import "ol/ol.css";
import { Map, View } from "ol";
import TileLayer from "ol/layer/Tile";
import OSM from "ol/source/OSM";
import VectorLayer from "ol/layer/Vector";
import VectorSource from "ol/source/Vector";
import { fromLonLat, toLonLat, transform } from "ol/proj.js";
import Feature from "ol/Feature";
import { Point, LineString } from "ol/geom";
import { Style, Stroke, Fill, Text } from "ol/style";

// components
import Popup from "./Popup";
import Positions from "./Positions";

export default {
  name: "Map",
  components: {
    Popup,
    Positions
  },
  data() {
    return {
      map: null,
      markerSource: null,
      markerLayer: null,
      startMarkerStyle: null,
      goalMarkerStyle: null,
      edgeSource: null,
      edgeLayer: null,
      edgeStyle: null,
      sourceProjection: "EPSG:4326",
      targetProjection: "EPSG:3857",
      infoClick: null,
      startPositions: [],
      goalPosition: null,
      gettingCenter: false
    };
  },
  mounted() {
    this.initMap();
  },
  methods: {
    initMap() {
      this.map = new Map({
        target: "map",
        layers: [
          new TileLayer({
            source: new OSM()
          })
        ],
        view: new View({
          center: transform(
            [10, 51],
            this.sourceProjection,
            this.targetProjection
          ),
          zoom: 6
        })
      });

      // layer for the markers of the startPositions and the goalPosition
      this.markerSource = new VectorSource({
        features: []
      });
      this.markerLayer = new VectorLayer({
        source: this.markerSource,
        zIndex: 1001
      });
      this.map.addLayer(this.markerLayer);

      // style for the startMarkers
      this.startMarkerStyle = new Style({
        text: new Text({
          text: "\uf041",
          font: "normal 25px FontAwesome",
          fill: new Fill({
            color: "red"
          }),
          offsetY: -12
        })
      });

      // style for goalMarker
      this.goalMarkerStyle = new Style({
        text: new Text({
          text: "\uf041",
          font: "normal 35px FontAwesome",
          fill: new Fill({
            color: "#0ff207"
          }),
          stroke: new Stroke({
            color: "black"
          }),
          offsetY: -17
        })
      });

      // layer for the edges of the routes on the map
      this.edgeSource = new VectorSource({});
      this.edgesLayer = new VectorLayer({
        source: this.edgeSource,
        zIndex: 1000
      });
      this.map.addLayer(this.edgesLayer);

      // style for the edges
      this.edgeStyle = new Style({
        stroke: new Stroke({
          width: 6,
          color: "orange"
        })
      });

      // this.map.updateSize();

      // event listener for map clicks to add new start positions
      var self = this;
      this.map.on("singleclick", function(e) {
        // console.log("coords: " +  e.coordinate);
        self.infoClick = toLonLat(e.coordinate);
        // console.log(self.infoClick);
        self.getNearestNode(self.infoClick, "start");

        // $("#clicked-lat").text(lonlat[1]);
        // $("#clicked-lon").text(lonlat[0]);
        // $("#nearest-lat").text(info.coords.lat);
        // $("#nearest-lon").text(info.coords.lon);
        // $("#popup-header").text("clicked at");
        // popupOverlay.setPosition(coords);

        // fillMarkerTable(markerLonLat, info.id);
      });
    },
    /**
     * function to get nearest node to coordinates[]-array
     * @return an object with an object 'coords' with two objects ('lon', 'lat')
     */
    getNearestNode(lonlat, startGoal) {
      var self = this;
      axios({
        method: "post",
        url: window.serverUrl + "getnearestnode",
        data: '{"lon": ' + lonlat[0] + ', "lat": ' + lonlat[1] + "}"
      })
        .then(res => {
          console.log("nearestNode response: ", res.data);
          self.addNewMarker(res.data, startGoal);
        })
        .catch(err => {
          console.error(err);
        });
    },

    /**
     * add a new marker to the map and add the position to the startPositions-array or write it to the goalPosition
     */
    addNewMarker(newPos, startGoal) {
      let markerLonLat = [newPos.coords.lon, newPos.coords.lat];

      var marker = new Feature({
        type: "geoMarker",
        geometry: new Point(fromLonLat(markerLonLat)),
        name: "marker" + this.markerSource.getFeatures().length
      });
      var featureId = this.markerSource.getFeatures().length;
      marker.setId(featureId);

      if (startGoal == "goal") {
        marker.setStyle(this.goalMarkerStyle);
      } else {
        marker.setStyle(this.startMarkerStyle);
      }
      console.log("features: " + this.markerSource.getFeatures().length);
      this.markerSource.addFeature(marker);

      // add featureId to newPos to save it in startPositions - needed for removal
      newPos.featureId = featureId;
      if (startGoal == "goal") {
        this.goalPosition = newPos;
        // this.getPathsFromDijkstra();
      } else {
        newPos.weight = 1;
        this.startPositions.push(newPos);
      }
    },

    /**
     * remove a position from startPositions and the according marker on the map
     */
    onRemovePos(posIndex) {
      var featureId = this.startPositions[posIndex].featureId;
      this.markerSource.removeFeature(
        this.markerSource.getFeatureById(featureId)
      );
      this.startPositions.splice(posIndex, 1);
    },

    /**
     * Get the coordinates of the center of the current startPositions.
     * Get the nearest node according to the coordinates and save it to goalPositions (done by getNearestNode()).
     */
    onGetCenter() {
      // make button display loading
      this.gettingCenter = true;

      // calculate center position with coordinates
      var lat = 0;
      var lon = 0;
      var totWeight = 0;
      this.startPositions.forEach(element => {
        lat += element.coords.lat * element.weight;
        lon += element.coords.lon * element.weight;
        totWeight += element.weight;
      });
      lat = lat / totWeight;
      lon = lon / totWeight;
      var center = [lon, lat];

      // find nearest node to the calculated center
      this.getNearestNode(center, "goal");
    },

    /**
     * build data (JSON) to call dijkstra and call dijkstra
     */
    getPathsFromDijkstra() {
      // build data for dijkstra
      // var data = '{"centerId": ' + this.goalPosition.id;
      var data = "{";
      for (var i in this.startPositions) {
        data += '"id' + i + '": ' + this.startPositions[i].id + ", ";
      }
      data = data.substring(0, data.length - 2) + "}";

      var self = this;
      axios({
        method: "POST",
        url: window.serverUrl + "getpaths",
        data: data
      })
        .then(res => {
          console.log(res.data);
          // call function to show the center calculated by the dijkstras
          self.showCenter(res.data.center);
          // call func to show routes from dijkstras on map in edge-layer
          self.showPaths(res.data.paths);
        })
        .catch(err => {
          console.error(err);
        });
    },

    /**
     *
     */
    showCenter(center) {
      this.addNewMarker(center, "goal");
    },

    /**
     * Show the routes on the map.
     * Therefore, make new edges and add them to the edge-layer
     */
    showPaths(paths) {
      for (var path in paths) {
        var line = [];
        for (var segment in paths[path]) {
          line.push(
            fromLonLat([paths[path][segment].lon, paths[path][segment].lat])
          );
        }
        var edge = new Feature({
          type: "edge",
          geometry: new LineString(line),
          name: "path"
        });
        edge.setStyle(this.edgeStyle);
        this.edgeSource.addFeature(edge);
      }

      // when done stop button displaying loading
      this.gettingCenter = false;
    },

    /**
     * Func to clear all elements on all layers
     */
    onClearMap() {
      this.startPositions = [];
      this.markerSource.clear();
      this.edgeSource.clear();
    }
  }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
#map {
  height: 100%;
  width: 100%;
}
</style>
