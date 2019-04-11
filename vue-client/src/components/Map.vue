<template>
  <div>
    <vl-map
      :load-tiles-while-animating="true"
      :load-tiles-while-interacting="true"
      data-projection="EPSG:4326"
      style="height:800px"
    >
      <vl-view :zoom.sync="zoom" :center.sync="center" :rotation.sync="rotation"></vl-view>

      <vl-geoloc @update:position="geoloPosition = $event">
        <template slot-scope="geoloc">
          <vl-feature v-if="geoloc.position" id="position-feature">
            <vl-geom-point :coordinates="geoloc.position"></vl-geom-point>
            <vl-style-box>
              <vl-style-icon src="_media/marker.png" :scale="0.4" :anchor="[.5, 1]"></vl-style-icon>
            </vl-style-box>
          </vl-feature>
        </template>
      </vl-geoloc>

      <vl-layer-tile id="osm">
        <vl-source-osm></vl-source-osm>
      </vl-layer-tile>
    </vl-map>

    <div style="padding: 20px">
      Zoom: {{ zoom }}
      <br>
      Center: {{ center }}
      <br>
      Rotation: {{ rotation }}
      <br>
      My geolocation: {{ geolocPosition }}
    </div>
  </div>
</template>

<script>
import Vue from "vue";
import VueLayers from "vuelayers";
import "vuelayers/lib/style.css";

Vue.use(VueLayers);

export default {
  name: "Map",
  data() {
    return {
      zoom: 2,
      center: [0, 0],
      rotation: 0,
      geolocPosition: undefined
    };
  }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
/* h3 {
  margin: 40px 0 0;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}
a {
  color: #42b983;
} */
</style>
