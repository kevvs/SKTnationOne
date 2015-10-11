$(document).ready(function() {
  //$("#main-graph").html("Hello World!");
    var width = 500,
        height = 500,
        cellSize = 50; // cell size
  //function show2DLayer(data) {
      var nCol = 4, nRow = 4;
      var data = [-1.43275,	-5.48564,	0.418131,	-6.33078,
                  -3.36661,	8.07187,	-10.4037,	42.2142,
                  12.3066,	-11.7717,	80.9198,	-53.1436,
                  -12.9868,	28.4935,	-56.3632,	-10.0251],
          colors = ["#8f00ff", "#4b0082", "#0000ff", "#ffff00", "#ff7f00", "#ff0000"];

      var svg = d3.select("#main-graph")
                    .append("svg")
                    .attr("width", width)
                    .attr("height", height);

      var colorScale = d3.scale.quantile()
                    .domain([-100, 100])
                    .range(colors);

      var rect = svg.selectAll(".cell")
                  .data(data)
                .enter().append("rect")
                  .attr("class", "cell")
                  .attr("width", cellSize)
                  .attr("height", cellSize)
                  .attr("x", function(d ,i) { return  (i % nCol) * cellSize + 2; })
                  .attr("y", function(d, i) { return   Math.floor(i/nCol)* cellSize + 2; })
                  .style("fill", function(d) { return colorScale(d); });
      

});
