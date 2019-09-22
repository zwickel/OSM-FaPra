var baseUrl = "http://localhost:8091/";

var map = new ol.Map({
  target: "map",
  layers: [
    new ol.layer.Tile({
      source: new ol.source.OSM()
    })
  ],
  view: new ol.View({
    center: ol.proj.fromLonLat([9.1066, 48.74518]),
    zoom: 5
  })
});

// popup ##############################################################
/**
 * Elements that make up the popup.
 */
var container = document.getElementById("popup");
var content = document.getElementById("popup-content");
var closer = document.getElementById("popup-closer");

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

// marker section ##############################################################
/**
 * style for markers on map
 */
var markerStyle = new ol.style.Style({
  text: new ol.style.Text({
    text: "\uf041",
    font: "normal 25px FontAwesome",
    fill: new ol.style.Fill({
      color: "red"
    })
  })
});

/**
 * create a source that will contain markers
 */
var markerSource = new ol.source.Vector({
  features: []
  // features: [marker]
});

/**
 * create a layer that will contain the source with the markers
 */
var markerLayer = new ol.layer.Vector({
  source: markerSource
});

/**
 * add the layer to the map
 */
map.addLayer(markerLayer);

// edge section ################################################################

/**
 * style for edge on map
 */
var edgeStyle = new ol.style.Style({
  stroke: new ol.style.Stroke({
    width: 6,
    color: "orange"
  })
});

/**
 * create source that will contain the edge
 */
var edgeSource = new ol.source.Vector({
  // features: [edge]
});

/**
 * create a layer
 */
var edgesLayer = new ol.layer.Vector({
  source: edgeSource
});

map.addLayer(edgesLayer);

/**
 * function to get nearest node to coordinates[]-array
 * @return an object with an object 'coords' with two objects ('lon', 'lat')
 */
function getNearestNode(lonlat) {
  var info;

  $.ajax({
    method: "POST",
    async: false,
    url: baseUrl + "getnearestnode",
    data: '{"lon": ' + lonlat[0] + ', "lat": ' + lonlat[1] + "}"
  }).done(function(data, status, xhr) {
    console.log("nearestNode response: " + data);
    info = JSON.parse(data);
  });

  return info;
}

map.on("singleclick", function(e) {
  var coords = e.coordinate;
  // console.log("coords: " + coords);
  var lonlat = ol.proj.toLonLat(coords);
  // console.log(lonlat);
  var info = getNearestNode(lonlat);

  $("#clicked-lat").text(lonlat[1]);
  $("#clicked-lon").text(lonlat[0]);
  $("#nearest-lat").text(info.coords.lat);
  $("#nearest-lon").text(info.coords.lon);
  $("#popup-header").text("clicked at");
  popupOverlay.setPosition(coords);

  var markerLonLat = [info.coords.lon, info.coords.lat];

  var marker = new ol.Feature({
    type: "geoMarker",
    geometry: new ol.geom.Point(ol.proj.fromLonLat(markerLonLat)),
    name: "marker" + markerSource.getFeatures().length
  });
  marker.setStyle(markerStyle);
  console.log("features: " + markerSource.getFeatures().length);
  markerSource.addFeature(marker);

  fillMarkerTable(markerLonLat, info.id);
});

var positions = [];

// function fills the position-table and the position-array
function fillMarkerTable(lonlatArr, nodeId) {
  // add to Positions-tab
  $("#position-popup-content").html(function() {
    return (
      $("#position-popup-content").html() +
      '<table id="position' +
      positions.length +
      '" class="position-table"><tr><th colspan="2">Position ' +
      positions.length +
      "</th></tr><tr><td>lat</td><td>" +
      lonlatArr[1] +
      "</td></tr><tr><td>lon</td><td>" +
      lonlatArr[0] +
      "</td></tr><tr><td colspan='2'><button type='button' onclick='removePosition(" +
      positions.length +
      ");' style='width:100%;'>Remove Position</button></td></tr>"
    );
  });

  // add to positions-array
  positions.push({
    id: nodeId,
    lon: lonlatArr[0],
    lat: lonlatArr[1],
    weight: 1
  });
  console.log(positions);

  // show Positions-tab
  if ($("#position-popup").css("display") == "none") {
    $("#position-popup").show();
  }
}

function removePosition(id) {
  // delete according feature (marker)
  var featureToRemove = markerSource.getClosestFeatureToCoordinate(
    ol.proj.fromLonLat([positions[id].lon, positions[id].lat])
  );

  markerSource.removeFeature(featureToRemove);

  // delete from positions-array
  positions.splice(id, 1);
  $("#position" + id).remove();

  // update positions-table
  for (var i = id; i < positions.length; i++) {
    var j = i + 1;
    $("#position" + j + " th").html("Position " + i);
    $("#position" + j + " button").attr(
      "onclick",
      "removePositionn(" + i + ");"
    );
    $("#position" + j).attr("id", "positionn" + i);
  }
}

function deletePositions() {
  markerSource.clear();
  popupOverlay.setPosition(undefined);
  $("#position-popup").hide();
  positions = [];
  $("#position-popup-content").html("");
}

function getCenter() {
  // calculate center of positions
  var lat = 0;
  var lon = 0;
  var totWeight = 0;
  positions.forEach(element => {
    lat += element.lat * element.weight;
    lon += element.lon * element.weight;
    totWeight += element.weight;
  });
  lat = lat / totWeight;
  lon = lon / totWeight;
  center = [lon, lat];

  // get nearest node to center
  var nearestNodeToCenter = getNearestNode(center);

  // if (nearestNodeToCenter.id != -1)

  // build new marker and place on map
  var marker = new ol.Feature({
    type: "geoMarker",
    geometry: new ol.geom.Point(
      ol.proj.fromLonLat([
        nearestNodeToCenter.coords.lon,
        nearestNodeToCenter.coords.lat
      ])
    ),
    name: "center"
  });
  marker.setStyle(
    new ol.style.Style({
      text: new ol.style.Text({
        text: "\uf041",
        font: "normal 35px FontAwesome",
        stroke: new ol.style.Stroke({
          color: "black"
        }),
        fill: new ol.style.Fill({
          color: "#0ff207"
        })
      })
    })
  );
  markerSource.addFeature(marker);

  // build data for dijkstra
  var data = '{"centerId": ' + nearestNodeToCenter.id;
  for (i in positions) {
    data += ', "id' + i + '": ' + positions[i].id;
  }
  data += "}";

  // ajax call to calculate directions (dijkstra)
  $.ajax({
    method: "POST",
    url: baseUrl + "getpaths",
    data: data
  }).done(function(res, status, xhr) {
    console.log(res);
    paths = JSON.parse(res);

    for (var path in paths) {
      var line = [];
      for (var segment in paths[path]) {
        console.log("in segment");
        line.push(
          ol.proj.fromLonLat([
            paths[path][segment].lon,
            paths[path][segment].lat
          ])
        );
      }
      var edge = new ol.Feature({
        type: "edge",
        geometry: new ol.geom.LineString(line),
        name: "path"
      });
      edge.setStyle(edgeStyle);
      edgeSource.addFeature(edge);
    }
  });
}
