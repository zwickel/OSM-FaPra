<template>
  <v-app>
    <v-app-bar id="toolbar" app dark>
      <v-toolbar-title class="headline text-uppercase">
        <span>OpenStreetMap</span>
        <v-spacer></v-spacer>
        <span class="font-weight-light">Meeting point finder</span>
      </v-toolbar-title>
      <v-spacer></v-spacer>
      <v-btn text>
        <span class="mr-2">Latest Release</span>
      </v-btn>
    </v-app-bar>

    <v-content>
      <Map />
    </v-content>
  </v-app>
</template>

<script>
import Map from "./components/Map";

export default {
  name: "App",
  components: {
    Map
  },
  data: () => ({
    //
  })
};
</script>
