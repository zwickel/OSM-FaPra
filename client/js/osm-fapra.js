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

/**
 * create a marker
 */
var marker = new ol.Feature({
  type: 'geoMarker',
  geometry: new ol.geom.Point([0,0]),
  name: 'nearest node to click'
});

/**
 * style for marker on map
 */
var markerStyle = new ol.style.Style({
  text: new ol.style.Text({
    text: '\uf041',
    font: 'normal 25px FontAwesome',
    fill: new ol.style.Fill({
      color: 'red'
    })
  })
});

/**
 * append style to marker
 */
marker.setStyle(markerStyle);

/**
 * create a source that will contain marker
 */
var markerSource = new ol.source.Vector({
  features: [marker]
});

/**
 * create a layer that will contain the source with the marker
 */
var markerLayer = new ol.layer.Vector({
  source : markerSource
});

/**
 * add the layer to the map
 */
map.addLayer(markerLayer);

// var edgesLayer = new ol.layer.VectorLayer({
//   source: vectorSource
// });

// map.addLayer(edgesLayer);

map.on('singleclick', function(e) {
  var coords = e.coordinate;
  var lonlat = ol.proj.toLonLat(coords);
  console.log(lonlat);
  var info;

  $.ajax({
    method: 'POST',
    url: 'http://localhost:8091/getnearestnode',
    data: '{"lon": ' + lonlat[0] + ', "lat": ' + lonlat[1] + '}'
  }).done(function(data, status, xhr) {
    console.log(data);
    info = JSON.parse(data);
    $('#clicked-lat').text(lonlat[1]);
    $('#clicked-lon').text(lonlat[0]);
    $('#nearest-lat').text(info.coords.lat);
    $('#nearest-lon').text(info.coords.lon);
    $('#popup-header').text('clicked at')
    popupOverlay.setPosition(coords);
    marker.setGeometry(new ol.geom.Point(ol.proj.fromLonLat([info.coords.lon, info.coords.lat])));
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
