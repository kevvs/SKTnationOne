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
            var group = svg.append("g");
            group.selectAll(".cell")
                .data(data)
              .enter().append("rect")
                .attr("class", "cell")
                .attr("width", cellSize)
                .attr("height", cellSize)
                .attr("x", function(d ,i) { return  (i % nCol) * cellSize  + padding; }) //TODO СЃРґРµР»Р°С‚СЊ С‡РµСЂРµР· x.domain
                .attr("y", function(d, i) { return   Math.floor(i/nCol)* cellSize  + padding; })
                .style("fill", function(d) { return colorScale(d); });
            group = svg.append("g");
            var data2 = [-1.43275,	-5.48564,	0.418131,	-6.33078,
                  -3.36661,	8.07187,	-10.4037,	42.2142,
                  12.3066,	-11.7717,	80.9198,	-53.1436,
                  -12.9868,	28.4935,	-56.3632,	-10.0251];
            group.selectAll(".cell")
                .data(data2)
              .enter().append("rect")
                .attr("class", "cell")
                .attr("width", cellSize)
                .attr("height", cellSize)
                .attr("x", function(d ,i) { return  cellSize * nCol + (i % nCol) * cellSize  + 2*padding; }) //TODO СЃРґРµР»Р°С‚СЊ С‡РµСЂРµР· x.domain
                .attr("y", function(d, i) { return   Math.floor(i/nCol)* cellSize  + padding; })
                .style("fill", function(d) { return colorScale(d); });

        }
        function Legend() {
            var legend = svg.append("g");

            legend.selectAll(".legend")
                .data(colors.reverse()).enter()
                .append("rect")
                .attr("class", "legend")
                .attr("x", function(d, i) { return nRow * cellSize * 2 + 3*padding;})
                .attr("y", function(d, i) { return cellSize * i / 2 + 2*padding;})
                .attr("width", cellSize / 2)
                .attr("height", cellSize / 2)
                .style("fill", function(d, i) { return d; });

            legend.selectAll(".legend-text")
                .data([-100].concat(colorScale.quantiles()).concat(100).reverse()).enter()
                .append("text")
                .attr("class", "legend-text")
                .text(function(d, i) { return Math.ceil(d * 100)/100; })
                .attr("x", function(d, i) { return nRow * cellSize * 2 + 3.5*padding + cellSize/2; })
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

                // generate data for area
                var xareaData = [], yareaData = [];
                for(var i = Math.round(min_x/step_x); i < Math.round(max_x/step_x); i++)
                    xareaData.push(i*step_x);
                for(var i = Math.round(min_y/step_y); i < Math.round(max_y/step_y); i++)
                    yareaData.push(i*step_y);

                // draw area for x
                var grouparea = svg.append("g");
                grouparea.selectAll(".vlines")
                    .data(xareaData).enter()
                    .append("line")
                    .attr("class","vlines")
                    .style("stroke-dasharray", ("1, 3"))
                    .attr("x1", function(d,i){ return x(d);})
                    .attr("y1", 0)
                    .attr("x2", function(d,i){ return x(d);})
                    .attr("y2", height);

                // draw area for y
                grouparea.selectAll(".hlines")
                    .data(yareaData).enter()
                    .append("line")
                    .attr("class","hlines")
                    .style("stroke-dasharray", ("1, 3"))
                    .attr("x1", 0)
                    .attr("y1", function(d,i){ return y(d);})
                    .attr("x2", width)
                    .attr("y2", function(d,i){ return y(d);});

                // draw zero-area
                grouparea.selectAll(".vmainline")
                    .data([x(0)]).enter()
                    .append("line")
                    .attr("class","vmainline")
                    .attr("x1", function(d,i){ return d;})
                    .attr("y1", 0)
                    .attr("x2", function(d,i){ return d;})
                    .attr("y2", height);

                grouparea.selectAll(".hmainline")
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

                // draw labels for area
                var groupLabelsarea = svg.append("g");
                groupLabelsarea.selectAll(".x-area-text")
                    .data(xareaData).enter()
                    .append("text")
                    .attr("class", "x-area-text")
                    .text(function(d, i) { return d; })
                    .attr("x", function(d, i) { return x(d) + 5; })
                    .attr("y", function(d, i) { return y(0) - 5; });

                groupLabelsarea.selectAll(".y-area-text")
                    .data(yareaData).enter()
                    .append("text")
                    .attr("class", "y-area-text")
                    .text(function(d, i) { return d == 0 ? '': d; })
                    .attr("x", function(d, i) { return x(0) + 5; })
                    .attr("y", function(d, i) { return y(d) - 5; });
            });
        }
    }

    function UpdateTable(nCol, nRow, table) {
        var tbody = table.find("tbody").html("");
        nRow = parseInt(nRow);
        var input_w = table.width()/nCol;
        for(var i = 0; i < nRow; i++) {
            var tr = $("<tr/>").appendTo(tbody);
            for(var j = 0; j < nCol; j++) {
                tr.append($("<td/>").append($("<input/>", {type: "text"}).css("width", input_w)));
            }
        }
    }

    var $n_recvs = $("#n_receivers"),
        $incnum  = $("#incnum"),
        $usr_param_reg = $(".use_parap_reg"),
        $main_graph = $("#main-graph"),
        $includes =  $("#includes"),
        $receivers = $("#receivers"),
        $example_li = $("#examples li"),
        $form = $("#task");

    function defaultForm() {
        $n_recvs.val(1);
        $n_recvs.trigger("input");
        $incnum.val(1);
        $incnum.trigger("input");
        $usr_param_reg.click();
    }

    $n_recvs.bind('keyup input', function() { UpdateTable(3, $(this).val(), $receivers); });
    $incnum.bind('keyup input', function() { UpdateTable(9, $(this).val(), $includes); });

    $usr_param_reg.click(function() {
        var ctx = $(this);
        ctx.parent("fieldset")
            .find("input")
            .not("input[name='"+ctx.attr("name")+"']")
            .prop("disabled", !ctx.prop("checked"));
    });
    $example_li.click(function() {
        $.ajax({
            dataType: "json",
            url: "json/" + $(this).attr("data-file"),
            success: function(test) {
                    setTest(test);
            }
        });
    });
    $form.bind('reset', function(e) {
        e.preventDefault();
        $form.find("input[type='number']").val("");
        $form.find("input[type='text']").val("");
        $usr_param_reg.prop("checked", false);
        defaultForm();
    });

    $form.bind('submit', function(e) {
        e.preventDefault();
        var test = getTest();
        //TODO make request "/task"
        console.log(JSON.stringify(test));
    });

    function getTest() {
        var data = {area: [], includes: [], receivers: [], config: {}};
        var checked_group = $usr_param_reg.map(function() {
                                    if($(this).prop("checked"))
                                        return $(this).attr("name");
                                }).get();
        ['xmin', 'xmax', 'ymin', 'ymax','zmin', 'zmax', 'xnum', 'ynum', 'znum'].forEach(function(t) {
            data.area.push($form.find("input[name='"+t+"']").val());
        });
        data.config    = $.extend({alpha: [] , gamma: []}, getValFromGroups(checked_group, ['0', '_step', '_coeff']));
        data.includes  = getValFromTable($includes);
        data.receivers = getValFromTable($receivers);
        function getValFromGroups(groups_names, fields) {
            var groups = {};
            groups_names.forEach(function(name) {
                groups[name] = fields.map(function(m) {
                         return $form.find("input[name='" + name + m + "']").val();
                    });
            });
            return groups;
        }
        function getValFromTable(table) {
            var info = [];
            table.find("tbody tr").each(function() {
                info.push($(this).find("input")
                                 .map(function() {
                                    return $(this).val();
                                 })
                                 .get());
            });
            return info;
        }
        return data;
    }
    function setTest(test) {
        $form.trigger('reset');

        //setValForGroups(test.area, ['min', 'max', 'num']);
        ['xmin', 'xmax', 'ymin', 'ymax','zmin', 'zmax', 'xnum', 'ynum', 'znum'].forEach(function(t, i) {
            $form.find("input[name='"+t+"']").val(test.area[i]);
        });
        setValForGroups(test.config, ['0', '_step', '_coeff']);

        Object.keys(test.config).forEach(function(k) {
            if(test.config[k].length == 0)
                $form.find("input[name=" + k + "]").click();
        });

        $incnum.val(test.includes.length);
        $incnum.trigger("input");
        setValForTable(test.includes, $includes);

        $n_recvs.val(test.receivers.length);
        $n_recvs.trigger("input");
        setValForTable(test.receivers, $receivers);
        function setValForGroups(arr, fields) {
            /* Example:
                    arr: {"x":[1,2,3], "y":[4,5,6], "z":[7,8,9]},
                    groups_fields: [min, 'max', 'num']
            */
            Object.keys(arr).forEach(function(k) {
                fields.forEach(function(f, i) {
                    $form.find("input[name='" + k + f + "']").val(arr[k][i]);
                });
            });
        }
        function setValForTable(arr, table) {
            table.find("tbody tr").each(function(i) {
                $(this).find("input").each(function(j) {
                    $(this).val(arr[i][j]);
                });
            });
        }
    }

    // !!! TODO uncomment to make all fields of form as 'required'
    $form.find("input[type='number']").prop("required", true);
    $form.find("input[type='text']").prop("required", true);
    defaultForm();
    PlotsD3($main_graph.width(), $main_graph.height());
});
