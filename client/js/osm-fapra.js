var map = new ol.Map({
  target: 'map',
  layers: [
    new ol.layer.Tile({
      source: new ol.source.OSM()
    })
  ],
  view: new ol.View({
    center: ol.proj.fromLonLat([9.10660, 48.74518]),
    zoom: 5
  })
});

$.ajax({
  url: "http://localhost:8091/match/8"
}).done(function(data, status, xhr) {
  // alert("ajax done");
  alert(data);
  // $( this ).addClass( "done" );
});
