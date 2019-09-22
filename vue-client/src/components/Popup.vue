<template>
  <div>
    <div id="popup"></div>
  </div>
</template>

<script>
import Overlay from "ol/Overlay.js";

export default {
  name: "Popup",
  data() {
    return {
      popup: null
    };
  },
  mounted() {
    this.popup = new Overlay({
      element: document.getElementById("popup")
    });
  }
};
</script>

<style>
</style>
