$(document).ready(function() {
  //$("#main-graph").html("Hello World!");
    function PlotsD3 (w, h, task) {
        var padding = 20,
            width = w,
            height = h/2; // cell size
        //function show2DLayer(data) {
        var svg = d3.select("#main-graph")
                      .append("svg")
                      .attr("width", width)
                      .attr("height", height);
        var x = d3.scale.linear()
                 .range([5, width - 10]);
        var y = d3.scale.linear()
                 .range([5, height - 10]);
        ContourPlot();
        Graph();
        function ContourPlot() {
            var colors = ["#336b34", "#5cb85c", "#ffff00", '#ffd20f', "#ff7f00", "#ff0000"],
                colorScale, nCol, nRow, cellSize, min, max;
            Plot();
            Legend();
            function generateInputArea(min1, max1, min2, max2, n1, n2, inc) {
                var data = [], n = n1*n2,
                    step1 = (max1 - min1)/n1,
                    step2 = (max2 - min2)/n2;
                data = Array.apply(null, Array(n)).map(function() { return 0; });
                var ind1 = Array.apply(null, Array(n1-1)).map(function(_,i) { return i; }),
                    range1 = d3.scale.quantile()
                                  .domain([min1, max1])
                                  .range(ind1);
                var ind2 = Array.apply(null, Array(n2-1)).map(function(_,i) { return i; }),
                    range2 = d3.scale.quantile()
                                    .domain([min2, max2])
                                    .range(ind2);
                inc.forEach(function(t) {
                    t = ArrToFloat(t);
                    for(var x = t[0]; x < t[1]; x = x + step1) {
                        for(var z = t[4]; z < t[5]; z = z + step2) {
                            data[range1(x) + range2(z)*n1] = t[6];
                        }
                    }
                });
                return data;
            }
            function ArrToFloat(arr) {
                arr.forEach(function(d, i) {
                    arr[i] = parseFloat(d);
                });
                return arr;
            }
            function Plot() {
                var data1 = [], data2 = [];
                //var data = task.output;
                var area_param = ArrToFloat(task.input.json.area);

                data1 = generateInputArea(area_param[0], area_param[1],
                                         area_param[4], area_param[5],
                                         area_param[6], area_param[8],
                                         task.input.json.includes);

                task.output.forEach(function(t) {
                    data2 = data2.concat(ArrToFloat(t));
                });

                min = d3.min(data2);
                max = d3.max(data2);
                colorScale = d3.scale.quantile()
                                  .domain([min, max])
                                  .range(colors);
                nCol = area_param[6];
                nRow = area_param[8];

                cellSize = Math.min(Math.floor((height-2*padding)/nRow),  Math.floor((width/2-2*padding)/nCol));

                //var data = [-1.43275,	-5.48564,	0.418131,	-6.33078,
                //          -3.36661,	8.07187,	-10.4037,	42.2142,
                //          12.3066,	-11.7717,	80.9198,	-53.1436,
                //          -12.9868,	28.4935,	-56.3632,	-10.0251];
                //data = task.output;
                var group = svg.append("g");
                group.selectAll(".cell")
                    .data(data1)
                  .enter().append("rect")
                    .attr("class", "cell")
                    .attr("width", cellSize)
                    .attr("height", cellSize)
                    .attr("x", function(d ,i) { return  (i % nCol) * cellSize; }) //TODO сделать через x.domain
                    .attr("y", function(d, i) { return   Math.floor(i/nCol)* cellSize  + padding; })
                    .style("fill", function(d) { return colorScale(d); });
                group = svg.append("g");
                group.selectAll(".cell")
                    .data(data2)
                  .enter().append("rect")
                    .attr("class", "cell")
                    .attr("width", cellSize)
                    .attr("height", cellSize)
                    .attr("x", function(d ,i) { return  cellSize * nCol + (i % nCol) * cellSize  + padding; }) //TODO сделать через x.domain
                    .attr("y", function(d, i) { return   Math.floor(i/nCol)* cellSize  + padding; })
                    .style("fill", function(d) { return colorScale(d); });

            }
            function Legend() {
                var legend = svg.append("g");

                legend.selectAll(".legend")
                    .data(colors.reverse()).enter()
                    .append("rect")
                    .attr("class", "legend")
                    .attr("x", function(d, i) { return nCol * cellSize * 2 + 1.5*padding;})
                    .attr("y", function(d, i) { return cellSize * i  + 2*padding;})
                    .attr("width", cellSize / 2)
                    .attr("height", cellSize )
                    .style("fill", function(d, i) { return d; });

                legend.selectAll(".legend-text")
                    .data([min].concat(colorScale.quantiles()).concat(max).reverse()).enter()
                    .append("text")
                    .attr("class", "legend-text")
                    .text(function(d, i) { return Math.ceil(d * 100)/100; })
                    .attr("x", function(d, i) { return nCol * cellSize * 2 + 1.5*padding + cellSize/2; })
                    .attr("y", function(d, i) { return cellSize * i  + 2*padding; });
            }

        }
        function Graph() {
            svg = d3.select("#main-graph")
                    .append("svg")
                    .attr("width", width)
                    .attr("height", height);

            var line = d3.svg.line()
                        .x(function(d) { return x(d[0]); })
                        .y(function(d) { return y(d[1]); });
            var data_analytical = task.bx_analytical;
            data_analytical.forEach(function(d, i) {
                data_analytical[i] = [parseFloat(d[0]), parseFloat(d[1])];
            });
            var data_calculated = task.bx_calculated;
            data_calculated.forEach(function(d, i) {
                data_calculated[i] = [parseFloat(d[0]), parseFloat(d[1])];
            });
            var data = data_analytical;
            //d3.json('bx_analytical.json',function(error,data){
            //if (error) throw error;
            var min_x = d3.min(data, function(d) { return d[0]; }),
                max_x = d3.max(data, function(d) { return d[0]; }),
                min_y = d3.min(data, function(d) { return d[1]; }),
                max_y = d3.max(data, function(d) { return d[1]; }),
                step_x = (max_x - min_x)/16,
                step_y = (max_y - min_y)/16;

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

            // draw labels for area
            var groupLabelsarea = svg.append("g");
            groupLabelsarea.selectAll(".x-area-text")
                .data(xareaData).enter()
                .append("text")
                .attr("class", "x-area-text")
                .text(function(d, i) { return d == 0 ? "0" : Math.abs(d) > 0.1 ? d.toPrecision(3): d.toExponential(2); })
                .attr("x", function(d, i) { return x(d) + 3; })
                .attr("y", function(d, i) { return y(0) - 5; });
            groupLabelsarea
                .append("text")
                .attr("class", "x-area-text name-axis")
                .text("x")
                .attr("x", function(d, i) { return x(xareaData[xareaData.length-1]) + 5; })
                .attr("y", function(d, i) { return y(0) + 15; });

            groupLabelsarea.selectAll(".y-area-text")
                .data(yareaData).enter()
                .append("text")
                .attr("class", "y-area-text")
                .text(function(d, i) { d= -d; return d == 0 ? '': Math.abs(d) > 0.1 ? d.toPrecision(3): d.toExponential(2); })
                .attr("x", function(d, i) { return x(0) + 5; })
                .attr("y", function(d, i) { return y(d) - 3; });
            groupLabelsarea
                .append("text")
                .attr("class", "y-area-text name-axis")
                .text("Bx")
                .attr("x", function(d, i) { return x(0) - 20; })
                .attr("y", function(d, i) { return y(yareaData[0]) + 15; });

            // draw our plot
            svg.append("path")
                .datum(data_analytical)
                .attr("class", "line-analytical")
                .attr("x", padding)
                .attr("y", padding)
                .attr("d", line);

            // draw our plot
            svg.append("path")
                .datum(data_calculated)
                .attr("class", "line-calculated")
                //.style("stroke-dasharray", ("1, 3"))
                .attr("x", padding)
                .attr("y", padding)
                .attr("d", line);

            //});
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
        $form = $("#task"),
        $loader = $("#wrap-loader"),
        $form_legend = $("legend");

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
        ctx.closest("fieldset")
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
    $form_legend.click(function() {
        $(this).next("div").toggle();
    });
    $form.bind('reset', function(e) {
        e.preventDefault();
        $form.find("input[type='number']").val("");
        $form.find("input[type='text']").val("");
        $usr_param_reg.prop("checked", false);
        defaultForm();
    });
    //var timer;
    function getTask() {
        var timer = setInterval(function () {
            $.ajax({
    			type: "GET",
                dataType: "json",
                url: "/api/1.0/task",
                success: function(r) {
                    if(r.status == "finished") {
                        clearInterval(timer);
                        $loader.hide();
                        //alert("!!!");
                        PlotsD3($main_graph.width(), $main_graph.height(), r);
                        //console.log(r);
                    }
                }
            });
        }, 3000);
    }
    $form.bind('submit', function(e) {
        e.preventDefault();
        $loader.show();
        var test = getTest();
        //TODO make request "/task"
        //console.log(JSON.stringify(test));
        $.ajax({
            type: "POST",
            dataType: "json",
            url: "/api/1.0/task",
            data: test,
            success: function() {
                getTask();
            }
        });
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
        if(checked_group.indexOf("gamma") > -1) {
            data.config.gamma.push($form.find("input[name='gamma_diff']").val());
        }
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
        setValForGroups(test.config, ['0', '_step', '_coeff', '_diff']);

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
    $form.find("fieldset > div").hide();
    defaultForm();
    //PlotsD3($main_graph.width(), $main_graph.height());
});
