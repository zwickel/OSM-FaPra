<template>
  <v-card id="list-container" dark>
    <v-card-title id="list-container-title">Positions</v-card-title>
    <v-card-text id="list-of-positions" class="scroll">
      <span v-if="positions.length==0"></span>
      <v-card v-for="(position, index) in positions" :key="index" :elevation="22">
        <v-card-title>
          <div>Position {{ index }}</div>
          <v-btn absolute right icon color="orange" @click="$emit('removePos', index)">
            <v-icon>mdi-close</v-icon>
          </v-btn>
        </v-card-title>
        <v-card-text>
          lat: {{position.coords.lat}}
          <br />
          lon: {{position.coords.lon}}
        </v-card-text>
      </v-card>
    </v-card-text>
    <v-card-actions id="list-container-actions">
      <v-btn :loading="gettingCenter" @click="$emit('getCenter')">Find Meeting Point</v-btn>
      <v-btn @click="$emit('clearMap')">Clear Map</v-btn>
    </v-card-actions>
  </v-card>
</template>

<script>
// import { mapGetters, mapActions } from "vuex";
export default {
  name: "Positions",
  props: ["positions", "gettingCenter"],
  data() {
    return {};
  },
  mounted() {
    document.getElementById("list-container").style.height =
      window.innerHeight -
      document.getElementById("toolbar").clientHeight -
      20 +
      "px";
    document.getElementById("list-of-positions").style.height =
      document.getElementById("list-container").clientHeight -
      document.getElementById("list-container-title").clientHeight -
      document.getElementById("list-container-actions").clientHeight +
      "px";
  },
  methods: {},
  computed: {}
};
</script>

<style>
.scroll {
  overflow-y: scroll;
}
</style>
