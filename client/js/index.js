$(document).ready(function() {
  //$("#main-graph").html("Hello World!");
    function PlotsD3 (w, h) {
        var padding = 20,
            width = w,
            height = h/2; // cell size
        //function show2DLayer(data) {
        var colors = ["#336b34", "#5cb85c", "#ffff00", '#ffd20f', "#ff7f00", "#ff0000"];
        var svg = d3.select("#main-graph")
                      .append("svg")
                      .attr("width", width)
                      .attr("height", height);
        var nCol = 4, nRow = 4;
        var cellSize = Math.floor((height-2*padding)/nRow);
        var colorScale = d3.scale.quantile()
                      .domain([-100, 100])
                      .range(colors);
        var x = d3.scale.linear()
                 .range([5, width - 10]);
        var y = d3.scale.linear()
                 .range([5, height - 10]);
        ContourPlot();
        Legend();
        Graph();
        function ContourPlot() {
            var data = [-1.43275,	-5.48564,	0.418131,	-6.33078,
                      -3.36661,	8.07187,	-10.4037,	42.2142,
                      12.3066,	-11.7717,	80.9198,	-53.1436,
                      -12.9868,	28.4935,	-56.3632,	-10.0251];

            var rect = svg.selectAll(".cell")
                        .data(data)
                      .enter().append("rect")
                        .attr("class", "cell")
                        .attr("width", cellSize)
                        .attr("height", cellSize)
                        .attr("x", function(d ,i) { return  (i % nCol) * cellSize  + padding; }) //TODO сделать через x.domain
                        .attr("y", function(d, i) { return   Math.floor(i/nCol)* cellSize  + padding; })
                        .style("fill", function(d) { return colorScale(d); });

        }
        function Legend() {
            var legend = svg.append("g");

            legend.selectAll(".legend")
                .data(colors.reverse()).enter()
                .append("rect")
                .attr("class", "legend")
                .attr("x", function(d, i) { return nRow * cellSize + 2*padding;})
                .attr("y", function(d, i) { return cellSize * i / 2 + 2*padding;})
                .attr("width", cellSize / 2)
                .attr("height", cellSize / 2)
                .style("fill", function(d, i) { return d; });

            legend.selectAll(".legend-text")
                .data([-100].concat(colorScale.quantiles()).concat(100).reverse()).enter()
                .append("text")
                .attr("class", "legend-text")
                .text(function(d, i) { return Math.ceil(d * 100)/100; })
                .attr("x", function(d, i) { return (nRow + 0.5) * cellSize + 10+ 2*padding; })
                .attr("y", function(d, i) { return cellSize * i / 2  + 2*padding; });
        }
        function Graph() {
            svg = d3.select("#main-graph")
                    .append("svg")
                    .attr("width", width)
                    .attr("height", height);

            var line = d3.svg.line()
                        .x(function(d) { return x(d[0]); })
                        .y(function(d) { return y(d[1]); });

            d3.json('bx_analytical.json',function(error,data){
                if (error) throw error;
                var min_x = d3.min(data, function(d) { return d[0]; }),
                    max_x = d3.max(data, function(d) { return d[0]; }),
                    min_y = d3.min(data, function(d) { return d[1]; }),
                    max_y = d3.max(data, function(d) { return d[1]; }),
                    step_x = 500,
                    step_y = 0.04;

                x.domain([min_x, max_x]);
                y.domain([min_y, max_y]);

                // generate data for axis
                var xAxisData = [], yAxisData = [];
                for(var i = Math.round(min_x/step_x); i < Math.round(max_x/step_x); i++)
                    xAxisData.push(i*step_x);
                for(var i = Math.round(min_y/step_y); i < Math.round(max_y/step_y); i++)
                    yAxisData.push(i*step_y);

                // draw axis for x
                var groupAxis = svg.append("g");
                groupAxis.selectAll(".vlines")
                    .data(xAxisData).enter()
                    .append("line")
                    .attr("class","vlines")
                    .style("stroke-dasharray", ("1, 3"))
                    .attr("x1", function(d,i){ return x(d);})
                    .attr("y1", 0)
                    .attr("x2", function(d,i){ return x(d);})
                    .attr("y2", height);

                // draw axis for y
                groupAxis.selectAll(".hlines")
                    .data(yAxisData).enter()
                    .append("line")
                    .attr("class","hlines")
                    .style("stroke-dasharray", ("1, 3"))
                    .attr("x1", 0)
                    .attr("y1", function(d,i){ return y(d);})
                    .attr("x2", width)
                    .attr("y2", function(d,i){ return y(d);});

                // draw zero-axis
                groupAxis.selectAll(".vmainline")
                    .data([x(0)]).enter()
                    .append("line")
                    .attr("class","vmainline")
                    .attr("x1", function(d,i){ return d;})
                    .attr("y1", 0)
                    .attr("x2", function(d,i){ return d;})
                    .attr("y2", height);

                groupAxis.selectAll(".hmainline")
                    .data([y(0)]).enter()
                    .append("line")
                    .attr("class","hmainline")
                    .attr("x1", 0)
                    .attr("y1", function(d,i){ return d;})
                    .attr("x2", width)
                    .attr("y2", function(d,i){ return d;});

                // draw our plot
                svg.append("path")
                    .datum(data)
                    .attr("class", "line")
                    .attr("x", padding)
                    .attr("y", padding)
                    .attr("d", line);

                // draw labels for axis
                var groupLabelsAxis = svg.append("g");
                groupLabelsAxis.selectAll(".x-axis-text")
                    .data(xAxisData).enter()
                    .append("text")
                    .attr("class", "x-axis-text")
                    .text(function(d, i) { return d; })
                    .attr("x", function(d, i) { return x(d) + 5; })
                    .attr("y", function(d, i) { return y(0) - 5; });

                groupLabelsAxis.selectAll(".y-axis-text")
                    .data(yAxisData).enter()
                    .append("text")
                    .attr("class", "y-axis-text")
                    .text(function(d, i) { return d == 0 ? '': d; })
                    .attr("x", function(d, i) { return x(0) + 5; })
                    .attr("y", function(d, i) { return y(d) - 5; });
            });
        }
    }

    function UpdateTable(nCol, nRow, table) {
        var tbody = table.find("tbody").html("");
        nRow = parseInt(nRow);
        var input_w = $("#receivers").width()/nCol;
        for(var i = 0; i < nRow; i++) {
            var tr = $("<tr/>").appendTo(tbody);
            for(var j = 0; j < nCol; j++) {
                tr.append($("<td/>").append($("<input/>", {type: "text"}).css("width", input_w)));
            }
        }
    }
    $("#n_receivers").bind('keyup input', function(e){
        UpdateTable(3, $(this).val(), $("#receivers"));
    });
    $("#incnum").bind('keyup input', function(e){
        UpdateTable(9, $(this).val(), $("#includes"));
    });
    $(".use_parap_reg").click(function() { // TODO после сброса ресетом не обнуляет эти поля
        var ctx = $(this);
        ctx.parent("fieldset")
            .find("input")
            .not("input[name='"+ctx.attr("name")+"']")
            .prop("disabled", !ctx.prop("checked"));
    });
    $("#n_receivers").trigger("input");
    $("#incnum").trigger("input");
    $(".use_parap_reg").click();
    PlotsD3($("#main-graph").width(), $("#main-graph").height());
});
