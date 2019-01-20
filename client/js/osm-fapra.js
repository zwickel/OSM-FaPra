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

/**
 * Elements that make up the popup.
 */
var container = document.getElementById('popup');
var content = document.getElementById('popup-content');
var closer = document.getElementById('popup-closer');

/**
 * Create an overlay to anchor the popup to the map.
 */
var popupOverlay = new ol.Overlay({
  element: container,
  autoPan: true,
  autoPanAnimation: {
    duration: 250
  }
});

/**
 * add overlay to map
 */
map.addOverlay(popupOverlay);

/**
 * Add a click handler to hide the popup.
 * @return {boolean} Don't follow the href.
 */
closer.onclick = function() {
  popupOverlay.setPosition(undefined);
  closer.blur();
  return false;
};

// var nearestNodeFeature = new ol.Feature({
//   name: 'nearest node to click'
// });

// var nearestNodeStyle = new ol.style.Style({
//   image: new ol.style.Icon({
//     text: new ol.style.Text({
//       text: '\uf3c5',
//       font: 'normal 18px FontAwesome',
//     })
//   })
// });

// nearestNodeFeature.setStyle(nearestNodeStyle);

// var vectorSource = new ol.source.Vector({
//   features: [nearestNodeFeature]
// });

// var vectorLayer = new ol.layer.Vector({
//   source: vectorSource
// });

// map.addLayer(vectorLayer);

map.on('singleclick', function(e) {
  var coords = e.coordinate;
  var lonlat = ol.proj.toLonLat(coords);
  console.log(lonlat);
  var info;

  $.ajax({
    method: 'POST',
    url: 'http://localhost:8091/getnearesttaginfo',
    data: '{"lon": ' + lonlat[0] + ', "lat": ' + lonlat[1] + '}'
  }).done(function(data, status, xhr) {
    console.log(data);
    info = JSON.parse(data);
    $('#clicked-lat').text(lonlat[1]);
    $('#clicked-lon').text(lonlat[0]);
    $('#nearest-lat').text(info.coords.lat);
    $('#nearest-lon').text(info.coords.lon);
    popupOverlay.setPosition(coords);
    // nearestNodeFeature.setGeometry(new ol.geom.Point(ol.proj.fromLonLat([info.coords.lon, info.coords.lat])));
  });

});

// test get ajax
// $.ajax({
//   url: 'http://localhost:8091/match/8'
// }).done(function(data, status, xhr) {
//   // alert('ajax done');
//   alert(data);
//   // $( this ).addClass( "done" );
// });
