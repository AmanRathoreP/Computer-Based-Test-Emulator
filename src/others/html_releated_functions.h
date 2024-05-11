#pragma once
#include "basic_csv_functions.h"
#include "test_starting_info.h"
#include <rapidcsv.h>
#include <unordered_map>
#include <wx/string.h>

class htmlGenerator
{
public:
    void create_test_summary(rapidcsv::Document doc, std::string html_file_name, const test_info test_starting_data);
    void create_detailed_test_analysis(rapidcsv::Document doc, std::string html_file_name, const test_info test_starting_data);
    void create_detailed_validity_report(rapidcsv::Document doc, const std::string html_file_name, const std::string test_folder_path);
    void create_summary_validity_report(rapidcsv::Document doc, const std::string html_file_name, const std::string test_folder_path, const float marks_for_correct_answer, const float marks_for_wrong_answer);

private:
    bool inline write_to_file(std::string file_content, std::string file_name);

private:
    std::string summary_boiler_plate = R"(<style type="text/css">
    .tg td,
    .tg th {
        border: 1px solid #93a1a1;
        font-family: Arial, sans-serif;
        font-size: 14px;
        overflow: hidden;
        padding: 10px 5px;
        word-break: normal
    }

    .tg .tg-k5k6,
    .tg .tg-lqoy {
        background-color: #ffffc7;
        border-color: inherit;
        color: #002b36;
        font-family: "Arial Black", Gadget, sans-serif !important
    }

    .tg .tg-k5k6,
    .tg .tg-lqoy,
    .tg .tg-wgie {
        font-family: "Arial Black", Gadget, sans-serif !important
    }

    .tg td {
        background-color: #fdf6e3;
        color: #002b36
    }

    .tg th {
        background-color: #657b83;
        color: #fdf6e3;
        font-weight: 400
    }

    .tg .tg-k5k6 {
        font-size: 18px;
        text-align: left;
        vertical-align: top
    }

    .tg .tg-lqoy,
    .tg .tg-ojej {
        font-size: 36px;
        text-align: left
    }

    .tg .tg-lqoy {
        vertical-align: top
    }

    .tg .tg-wgie,
    .tg .tg-y1n6 {
        border-color: #6200c9;
        color: #6200c9;
        vertical-align: top
    }

    .tg .tg-ojej {
        vertical-align: top
    }

    .tg .tg-wgie {
        background-color: #cb0000;
        font-size: 18px;
        text-align: left
    }

    .tg .tg-swt6 {
        font-family: "Lucida Sans Unicode", "Lucida Grande", sans-serif !important;
        font-size: 20px;
        position: -webkit-sticky;
        position: sticky;
        text-align: center;
        top: -1px;
        vertical-align: top;
        will-change: transform
    }

    .tg .tg-cg4a,
    .tg .tg-y1n6 {
        background-color: #cb0000;
        font-family: "Arial Black", Gadget, sans-serif !important;
        text-align: left
    }

    .tg .tg-y1n6 {
        font-size: 36px
    }

    .tg .tg-38fl,
    .tg .tg-cg4a,
    .tg .tg-xh3r {
        border-color: inherit
    }

    .tg .tg-cg4a {
        font-size: 18px;
        vertical-align: top
    }

    .tg .tg-38fl,
    .tg .tg-hb07,
    .tg .tg-xh3r {
        font-family: "Lucida Sans Unicode", "Lucida Grande", sans-serif !important;
        font-size: 20px;
        text-align: center;
        top: -1px;
        vertical-align: top;
        will-change: transform
    }

    .tg .tg-hb07 {
        font-weight: 700;
        position: -webkit-sticky;
        position: sticky
    }

    .tg .tg-38fl {
        background-color: #ffffc7;
        color: #002b36;
        position: -webkit-sticky;
        position: sticky
    }

    .tg .tg-0463,
    .tg .tg-1pav {
        background-color: #69ee7f;
        color: #6200c9
    }

    .tg .tg-xh3r {
        position: -webkit-sticky;
        position: sticky
    }

    .tg .tg-cim1 {
        font-family: Arial, Helvetica, sans-serif !important;
        font-size: 18px;
        text-align: left;
        vertical-align: top
    }

    .tg .tg-1pav,
    .tg .tg-cjhk,
    .tg .tg-g9u4,
    .tg .tg-ojtw {
        font-family: "Arial Black", Gadget, sans-serif !important;
        text-align: left;
        vertical-align: top
    }

    .tg .tg-cjhk {
        background-color: #69ee7f;
        font-size: 18px
    }

    .tg .tg-1pav {
        border-color: #6200c9;
        font-size: 18px
    }

    .tg .tg-g9u4 {
        border-color: inherit;
        font-size: 18px
    }

    .tg .tg-ojtw {
        background-color: #69ee7f;
        font-size: 36px
    }

    .tg .tg-0463 {
        border-color: #6200c9;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 36px;
        text-align: left;
        vertical-align: top
    }

    .tg .tg-sa6g,
    .tg .tg-ss2y {
        border-color: inherit;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 36px;
        text-align: left;
        vertical-align: top
    }

    .tg .tg-ss2y {
        background-color: #cb0000
    }

    .tg-sort-header::-moz-selection {
        background: 0 0
    }

    .tg-sort-header::selection {
        background: 0 0
    }

    .tg-sort-header {
        cursor: pointer
    }

    .tg-sort-header:after {
        content: '';
        float: right;
        margin-top: 7px;
        border-width: 0 5px 5px;
        border-style: solid;
        border-color: #404040 transparent;
        visibility: hidden
    }

    .tg-sort-header:hover:after {
        visibility: visible
    }

    .tg-sort-asc:after,
    .tg-sort-asc:hover:after,
    .tg-sort-desc:after {
        visibility: visible;
        opacity: .4
    }

    .tg-sort-desc:after {
        border-bottom: none;
        border-width: 5px 5px 0
    }

    @media screen and (max-width:767px) {

        .tg,
        .tg col {
            width: auto !important
        }

        .tg-wrap {
            overflow-x: auto;
            -webkit-overflow-scrolling: touch;
            margin: auto 0
        }
    }
</style>

<script charset="utf-8">var TGSort = window.TGSort || function (n) { "use strict"; function r(n) { return n ? n.length : 0 } function t(n, t, e, o = 0) { for (e = r(n); o < e; ++o)t(n[o], o) } function e(n) { return n.split("").reverse().join("") } function o(n) { var e = n[0]; return t(n, function (n) { for (; !n.startsWith(e);)e = e.substring(0, r(e) - 1) }), r(e) } function u(n, r, e = []) { return t(n, function (n) { r(n) && e.push(n) }), e } var a = parseFloat; function i(n, r) { return function (t) { var e = ""; return t.replace(n, function (n, t, o) { return e = t.replace(r, "") + "." + (o || "").substring(1) }), a(e) } } var s = i(/^(?:\s*)([+-]?(?:\d+)(?:,\d{3})*)(\.\d*)?$/g, /,/g), c = i(/^(?:\s*)([+-]?(?:\d+)(?:\.\d{3})*)(,\d*)?$/g, /\./g); function f(n) { var t = a(n); return !isNaN(t) && r("" + t) + 1 >= r(n) ? t : NaN } function d(n) { var e = [], o = n; return t([f, s, c], function (u) { var a = [], i = []; t(n, function (n, r) { r = u(n), a.push(r), r || i.push(n) }), r(i) < r(o) && (o = i, e = a) }), r(u(o, function (n) { return n == o[0] })) == r(o) ? e : [] } function v(n) { if ("TABLE" == n.nodeName) { for (var a = function (r) { var e, o, u = [], a = []; return function n(r, e) { e(r), t(r.childNodes, function (r) { n(r, e) }) }(n, function (n) { "TR" == (o = n.nodeName) ? (e = [], u.push(e), a.push(n)) : "TD" != o && "TH" != o || e.push(n) }), [u, a] }(), i = a[0], s = a[1], c = r(i), f = c > 1 && r(i[0]) < r(i[1]) ? 1 : 0, v = f + 1, p = i[f], h = r(p), l = [], g = [], N = [], m = v; m < c; ++m) { for (var T = 0; T < h; ++T) { r(g) < h && g.push([]); var C = i[m][T], L = C.textContent || C.innerText || ""; g[T].push(L.trim()) } N.push(m - v) } t(p, function (n, t) { l[t] = 0; var a = n.classList; a.add("tg-sort-header"), n.addEventListener("click", function () { var n = l[t]; !function () { for (var n = 0; n < h; ++n) { var r = p[n].classList; r.remove("tg-sort-asc"), r.remove("tg-sort-desc"), l[n] = 0 } }(), (n = 1 == n ? -1 : +!n) && a.add(n > 0 ? "tg-sort-asc" : "tg-sort-desc"), l[t] = n; var i, f = g[t], m = function (r, t) { return n * f[r].localeCompare(f[t]) || n * (r - t) }, T = function (n) { var t = d(n); if (!r(t)) { var u = o(n), a = o(n.map(e)); t = d(n.map(function (n) { return n.substring(u, r(n) - a) })) } return t }(f); (r(T) || r(T = r(u(i = f.map(Date.parse), isNaN)) ? [] : i)) && (m = function (r, t) { var e = T[r], o = T[t], u = isNaN(e), a = isNaN(o); return u && a ? 0 : u ? -n : a ? n : e > o ? n : e < o ? -n : n * (r - t) }); var C, L = N.slice(); L.sort(m); for (var E = v; E < c; ++E)(C = s[E].parentNode).removeChild(s[E]); for (E = v; E < c; ++E)C.appendChild(s[v + L[E - v]]) }) }) } } n.addEventListener("DOMContentLoaded", function () { for (var t = n.getElementsByClassName("tg"), e = 0; e < r(t); ++e)try { v(t[e]) } catch (n) { } }) }(document)</script>

<div class="tg-wrap">
    <table id="tg-F1IWO" class="tg">
        <thead>
            <tr>
                <th class="tg-hb07">Section</th>
                <th class="tg-hb07">Answered</th>
                <th class="tg-swt6">Answered &amp; <br>marked for review</th>
                <th class="tg-swt6">Not answered</th>
                <th class="tg-swt6">Not answered &amp;<br>marked for review</th>
                <th class="tg-38fl">Not visited</th>
                <th class="tg-xh3r">Total questions</th>
            </tr>
        </thead>
        <tbody>)";

    std::string summary_replaceable_text_for_section = R"(<tr>
                <td class="tg-cim1">$(section_name)</td>
                <td class="tg-cjhk">$(answered)</td>
                <td class="tg-1pav">$(answered_scrutinize)</td>
                <td class="tg-cg4a">$(not_answered)</td>
                <td class="tg-wgie">$(not_answered_scrutinize)</td>
                <td class="tg-k5k6">$(not_visited)</td>
                <td class="tg-g9u4">$(total_questions)</td>
            </tr>)";

    std::string summary_replaceable_text_for_total = R"(<tr>
                <td class="tg-ojej">Total</td>
                <td class="tg-ojtw">$(answered)</td>
                <td class="tg-0463">$(answered_scrutinize)</td>
                <td class="tg-ss2y">$(not_answered)</td>
                <td class="tg-y1n6">$(not_answered_scrutinize)</td>
                <td class="tg-lqoy">$(not_visited)</td>
                <td class="tg-sa6g">$(total_questions)</td>
            </tr>
        </tbody>
    </table>
</div>)";

    std::string detailed_analysis_boiler_plate = R"(<style type="text/css">
    img {
        max-height: 800px;
        width: auto;
        height: auto;
    }
    .tg {
        border-collapse: collapse;
        border-color: #93a1a1;
        border-spacing: 0;
        margin: 0px auto;
    }

    .tg td {
        background-color: #fdf6e3;
        border-color: #93a1a1;
        border-style: solid;
        border-width: 1px;
        color: #002b36;
        font-family: Arial, sans-serif;
        font-size: 14px;
        overflow: hidden;
        padding: 10px 5px;
        word-break: normal;
    }

    .tg th {
        background-color: #657b83;
        border-color: #93a1a1;
        border-style: solid;
        border-width: 1px;
        color: #fdf6e3;
        font-family: Arial, sans-serif;
        font-size: 14px;
        font-weight: normal;
        overflow: hidden;
        padding: 10px 5px;
        word-break: normal;
    }

    .tg .tg-fj6v {
        background-color: #fd6864;
        border-color: inherit;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 36px;
        text-align: center;
        vertical-align: top
    }

    .tg .tg-50kp {
        background-color: #fd6864;
        border-color: inherit;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 16px;
        text-align: center;
        vertical-align: top
    }

    .tg .tg-rn6i {
        border-color: inherit;
        font-family: "Lucida Sans Unicode", "Lucida Grande", sans-serif !important;
        font-size: 20px;
        font-weight: bold;
        position: -webkit-sticky;
        position: sticky;
        text-align: center;
        top: -1px;
        vertical-align: top;
        will-change: transform
    }

    .tg .tg-4dnv {
        background-color: #34ff34;
        border-color: inherit;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 36px;
        text-align: center;
        vertical-align: top
    }

    .tg .tg-x4p3 {
        background-color: #34ff34;
        border-color: inherit;
        color: #6200c9;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 36px;
        text-align: center;
        text-decoration: underline;
        vertical-align: top
    }

    .tg .tg-a6kj {
        background-color: #fd6864;
        border-color: inherit;
        color: #6200c9;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 36px;
        text-align: center;
        text-decoration: underline;
        vertical-align: top
    }

    .tg .tg-jy6h {
        background-color: #34ff34;
        border-color: inherit;
        color: #6200c9;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 16px;
        text-align: center;
        text-decoration: underline;
        vertical-align: top
    }

    .tg .tg-x01x {
        background-color: #fd6864;
        border-color: inherit;
        color: #6200c9;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 16px;
        text-align: center;
        text-decoration: underline;
        vertical-align: top
    }

    .tg .tg-ixhd {
        border-color: inherit;
        font-family: "Lucida Sans Unicode", "Lucida Grande", sans-serif !important;
        font-size: 36px;
        font-weight: bold;
        position: -webkit-sticky;
        position: sticky;
        text-align: center;
        top: -1px;
        vertical-align: top;
        will-change: transform
    }

    .tg .tg-a6xf {
        background-color: #34ff34;
        border-color: inherit;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 16px;
        text-align: center;
        vertical-align: top
    }

    .tg .tg-d7tt {
        background-color: #ffffc7;
        border-color: inherit;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 16px;
        text-align: center;
        vertical-align: top
    }

    .tg .tg-2p7n {
        background-color: #ffffc7;
        border-color: inherit;
        font-family: "Arial Black", Gadget, sans-serif !important;
        font-size: 36px;
        text-align: center;
        vertical-align: top
    }

    .tg-sort-header::-moz-selection {
        background: 0 0
    }

    .tg-sort-header::selection {
        background: 0 0
    }

    .tg-sort-header {
        cursor: pointer
    }

    .tg-sort-header:after {
        content: '';
        float: right;
        margin-top: 7px;
        border-width: 0 5px 5px;
        border-style: solid;
        border-color: #404040 transparent;
        visibility: hidden
    }

    .tg-sort-header:hover:after {
        visibility: visible
    }

    .tg-sort-asc:after,
    .tg-sort-asc:hover:after,
    .tg-sort-desc:after {
        visibility: visible;
        opacity: .4
    }

    .tg-sort-desc:after {
        border-bottom: none;
        border-width: 5px 5px 0
    }

    @media screen and (max-width: 767px) {
        .tg {
            width: auto !important;
        }

        .tg col {
            width: auto !important;
        }

        .tg-wrap {
            overflow-x: auto;
            -webkit-overflow-scrolling: touch;
            margin: auto 0px;
        }
    }
</style>
<script charset="utf-8">var TGSort = window.TGSort || function (n) { "use strict"; function r(n) { return n ? n.length : 0 } function t(n, t, e, o = 0) { for (e = r(n); o < e; ++o)t(n[o], o) } function e(n) { return n.split("").reverse().join("") } function o(n) { var e = n[0]; return t(n, function (n) { for (; !n.startsWith(e);)e = e.substring(0, r(e) - 1) }), r(e) } function u(n, r, e = []) { return t(n, function (n) { r(n) && e.push(n) }), e } var a = parseFloat; function i(n, r) { return function (t) { var e = ""; return t.replace(n, function (n, t, o) { return e = t.replace(r, "") + "." + (o || "").substring(1) }), a(e) } } var s = i(/^(?:\s*)([+-]?(?:\d+)(?:,\d{3})*)(\.\d*)?$/g, /,/g), c = i(/^(?:\s*)([+-]?(?:\d+)(?:\.\d{3})*)(,\d*)?$/g, /\./g); function f(n) { var t = a(n); return !isNaN(t) && r("" + t) + 1 >= r(n) ? t : NaN } function d(n) { var e = [], o = n; return t([f, s, c], function (u) { var a = [], i = []; t(n, function (n, r) { r = u(n), a.push(r), r || i.push(n) }), r(i) < r(o) && (o = i, e = a) }), r(u(o, function (n) { return n == o[0] })) == r(o) ? e : [] } function v(n) { if ("TABLE" == n.nodeName) { for (var a = function (r) { var e, o, u = [], a = []; return function n(r, e) { e(r), t(r.childNodes, function (r) { n(r, e) }) }(n, function (n) { "TR" == (o = n.nodeName) ? (e = [], u.push(e), a.push(n)) : "TD" != o && "TH" != o || e.push(n) }), [u, a] }(), i = a[0], s = a[1], c = r(i), f = c > 1 && r(i[0]) < r(i[1]) ? 1 : 0, v = f + 1, p = i[f], h = r(p), l = [], g = [], N = [], m = v; m < c; ++m) { for (var T = 0; T < h; ++T) { r(g) < h && g.push([]); var C = i[m][T], L = C.textContent || C.innerText || ""; g[T].push(L.trim()) } N.push(m - v) } t(p, function (n, t) { l[t] = 0; var a = n.classList; a.add("tg-sort-header"), n.addEventListener("click", function () { var n = l[t]; !function () { for (var n = 0; n < h; ++n) { var r = p[n].classList; r.remove("tg-sort-asc"), r.remove("tg-sort-desc"), l[n] = 0 } }(), (n = 1 == n ? -1 : +!n) && a.add(n > 0 ? "tg-sort-asc" : "tg-sort-desc"), l[t] = n; var i, f = g[t], m = function (r, t) { return n * f[r].localeCompare(f[t]) || n * (r - t) }, T = function (n) { var t = d(n); if (!r(t)) { var u = o(n), a = o(n.map(e)); t = d(n.map(function (n) { return n.substring(u, r(n) - a) })) } return t }(f); (r(T) || r(T = r(u(i = f.map(Date.parse), isNaN)) ? [] : i)) && (m = function (r, t) { var e = T[r], o = T[t], u = isNaN(e), a = isNaN(o); return u && a ? 0 : u ? -n : a ? n : e > o ? n : e < o ? -n : n * (r - t) }); var C, L = N.slice(); L.sort(m); for (var E = v; E < c; ++E)(C = s[E].parentNode).removeChild(s[E]); for (E = v; E < c; ++E)C.appendChild(s[v + L[E - v]]) }) }) } } n.addEventListener("DOMContentLoaded", function () { for (var t = n.getElementsByClassName("tg"), e = 0; e < r(t); ++e)try { v(t[e]) } catch (n) { } }) }(document)</script>
<div class="tg-wrap">
    <table id="tg-ZWZmw" class="tg">
        <thead>
            <tr>
                <th class="tg-ixhd">Section</th>
                <th class="tg-rn6i">Question<br>Number</th>
                <th class="tg-ixhd">Questions</th>
                <th class="tg-rn6i">Assumed<br>Answer</th>
                <th class="tg-rn6i">Time<br>Taken<br>in minutes</th>
            </tr>
        </thead>
        <tbody>)";

    std::unordered_map<std::string, std::string> table_row_boiler_plates = {
        {"nv", "<tr><td class=\"tg-d7tt\">$(section_name)</td><td class=\"tg-2p7n\">$(question_number)</td><td class=\"tg-2p7n\"><img src=\"$(question_image_path)\" alt=\"Question Image\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-2p7n\">$(assumed_answer)</td><td class=\"tg-2p7n\">$(time_taken)</td></tr>"},
        {"n", "<tr><td class=\"tg-50kp\">$(section_name)</td><td class=\"tg-fj6v\">$(question_number)</td><td class=\"tg-fj6v\"><img src=\"$(question_image_path)\" alt=\"Question Image\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-fj6v\">$(assumed_answer)</td><td class=\"tg-fj6v\">$(time_taken)</td></tr>"},
        {"nr", "<tr><td class=\"tg-x01x\">$(section_name)</td><td class=\"tg-a6kj\">$(question_number)</td><td class=\"tg-a6kj\"><img src=\"$(question_image_path)\" alt=\"Question Image\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-a6kj\">$(assumed_answer)</td><td class=\"tg-a6kj\">$(time_taken)</td></tr>"},
        {"ar", "<tr><td class=\"tg-jy6h\">$(section_name)</td><td class=\"tg-x4p3\">$(question_number)</td><td class=\"tg-x4p3\"><img src=\"$(question_image_path)\" alt=\"Question Image\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-x4p3\">$(assumed_answer)</td><td class=\"tg-x4p3\">$(time_taken)</td></tr>"},
        {"a", "<tr><td class=\"tg-a6xf\">$(section_name)</td><td class=\"tg-4dnv\">$(question_number)</td><td class=\"tg-4dnv\"><img src=\"$(question_image_path)\" alt=\"Question Image\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-4dnv\">$(assumed_answer)</td><td class=\"tg-4dnv\">$(time_taken)</td></tr>"},
    };

    std::string detailed_analysis_of_validity_boiler_plate = R"(<style type="text/css">.tg .tg-50ki,.tg .tg-50kp,.tg .tg-fj6i,.tg .tg-fj6v{font-family:"Arial Black",Gadget,sans-serif!important;border-color:inherit}.tg .tg-a6ki,.tg .tg-a6kj,.tg .tg-jy6h,.tg .tg-x01i,.tg .tg-x01x,.tg .tg-x4p3{color:#6200c9;text-decoration:underline}.tg .tg-4dnv,.tg .tg-50ki,.tg .tg-50kp,.tg .tg-a6ki,.tg .tg-a6kj,.tg .tg-fj6i,.tg .tg-fj6v,.tg .tg-rn6i,.tg .tg-x4p3{text-align:center;vertical-align:top}.tg td,.tg th{border:1px solid #93a1a1;font-family:Arial,sans-serif;font-size:14px;overflow:hidden;padding:10px 5px;word-break:normal}.tg .tg-50kp,.tg .tg-fj6v{background-color:#fd6864}.tg .tg-50ki,.tg .tg-fj6i{background-color:red}.tg .tg-ixhd,.tg .tg-rn6i{font-weight:700;top:-1px;will-change:transform}img{max-height:800px;width:auto;height:auto}.tg{border-collapse:collapse;border-color:#93a1a1;border-spacing:0;margin:0 auto}.tg td{background-color:#fdf6e3;color:#002b36}.tg th{background-color:#657b83;color:#fdf6e3;font-weight:400}.tg .tg-fj6i,.tg .tg-fj6v{font-size:36px}.tg .tg-50ki,.tg .tg-50kp{font-size:16px}.tg .tg-4dnv,.tg .tg-x4p3{background-color:#34ff34}.tg .tg-rn6i{border-color:inherit;font-family:"Lucida Sans Unicode","Lucida Grande",sans-serif!important;font-size:20px;position:-webkit-sticky;position:sticky}.tg .tg-4dnv,.tg .tg-a6ki,.tg .tg-a6kj,.tg .tg-x4p3{border-color:inherit;font-family:"Arial Black",Gadget,sans-serif!important;font-size:36px}.tg .tg-a6kj{background-color:#fd6864}.tg .tg-a6ki{background-color:red}.tg .tg-jy6h,.tg .tg-x01i,.tg .tg-x01x{font-family:"Arial Black",Gadget,sans-serif!important;font-size:16px;border-color:inherit;text-align:center;vertical-align:top}.tg .tg-jy6h{background-color:#34ff34}.tg .tg-x01x{background-color:#fd6864}.tg .tg-x01i{background-color:red}.tg .tg-ixhd{border-color:inherit;font-family:"Lucida Sans Unicode","Lucida Grande",sans-serif!important;font-size:36px;position:-webkit-sticky;position:sticky;text-align:center;vertical-align:top}.tg .tg-2p7n,.tg .tg-a6xf,.tg .tg-d7tt{border-color:inherit;font-family:"Arial Black",Gadget,sans-serif!important;text-align:center;vertical-align:top}.tg .tg-a6xf,.tg .tg-d7tt{font-size:16px}.tg .tg-a6xf{background-color:#34ff34}.tg .tg-2p7n,.tg .tg-d7tt{background-color:#ffffc7}.tg .tg-2p7n{font-size:36px}.tg-sort-header::-moz-selection{background:0 0}.tg-sort-header::selection{background:0 0}.tg-sort-header{cursor:pointer}.tg-sort-header:after{content:'';float:right;margin-top:7px;border-width:0 5px 5px;border-style:solid;border-color:#404040 transparent;visibility:hidden}.tg-sort-header:hover:after{visibility:visible}.tg-sort-asc:after,.tg-sort-asc:hover:after,.tg-sort-desc:after{visibility:visible;opacity:.4}.tg-sort-desc:after{border-bottom:none;border-width:5px 5px 0}@media screen and (max-width:767px){.tg,.tg col{width:auto!important}.tg-wrap{overflow-x:auto;-webkit-overflow-scrolling:touch;margin:auto 0}}</style>
                                                                <script charset="utf-8">var TGSort=window.TGSort||function(r){"use strict";function L(n){return n?n.length:0}function c(n,t,r,e=0){for(r=L(n);e<r;++e)t(n[e],e)}function E(n){return n.split("").reverse().join("")}function b(n){var t=n[0];return c(n,function(n){for(;!n.startsWith(t);)t=t.substring(0,L(t)-1)}),L(t)}function D(n,t,r=[]){return c(n,function(n){t(n)&&r.push(n)}),r}var a=parseFloat;function n(t,o){return function(n){var e="";return n.replace(t,function(n,t,r){return e=t.replace(o,"")+"."+(r||"").substring(1)}),a(e)}}var e=n(/^(?:\s*)([+-]?(?:\d+)(?:,\d{3})*)(\.\d*)?$/g,/,/g),o=n(/^(?:\s*)([+-]?(?:\d+)(?:\.\d{3})*)(,\d*)?$/g,/\./g);function u(n){var t=a(n);return!isNaN(t)&&L(""+t)+1>=L(n)?t:NaN}function w(n){var t=[],a=n;return c([u,e,o],function(r){var e=[],o=[];c(n,function(n,t){t=r(n),e.push(t),t||o.push(n)}),L(o)<L(a)&&(a=o,t=e)}),L(D(a,function(n){return n==a[0]}))==L(a)?t:[]}function s(n){if("TABLE"==n.nodeName){for(var n=(s=[],i=[],function t(n,r){r(n),c(n.childNodes,function(n){t(n,r)})}(n,function(n){"TR"==(u=n.nodeName)?(a=[],s.push(a),i.push(n)):"TD"!=u&&"TH"!=u||a.push(n)}),[s,i]),t=n[0],p=n[1],h=L(t),n=1<h&&L(t[0])<L(t[1])?1:0,l=1+n,g=t[n],N=L(g),m=[],T=[],C=[],r=l;r<h;++r){for(var e=0;e<N;++e){L(T)<N&&T.push([]);var o=t[r][e],o=o.textContent||o.innerText||"";T[e].push(o.trim())}C.push(r-l)}c(g,function(n,d){m[d]=0;var v=n.classList;v.add("tg-sort-header"),n.addEventListener("click",function(){var u=m[d];!function(){for(var n=0;n<N;++n){var t=g[n].classList;t.remove("tg-sort-asc"),t.remove("tg-sort-desc"),m[n]=0}}(),(u=1==u?-1:+!u)&&v.add(0<u?"tg-sort-asc":"tg-sort-desc"),m[d]=u;var n,t,r,e,o=T[d],a=function(n,t){return u*o[n].localeCompare(o[t])||u*(n-t)},s=(L(e=w(n=o))||(t=b(n),r=b(n.map(E)),e=w(n.map(function(n){return n.substring(t,L(n)-r)}))),e);(L(s)||L(s=L(D(e=o.map(Date.parse),isNaN))?[]:e))&&(a=function(n,t){var r=s[n],e=s[t],o=isNaN(r),a=isNaN(e);return o&&a?0:o?-u:a||e<r?u:r<e?-u:u*(n-t)});var i,c=C.slice();c.sort(a);for(var f=l;f<h;++f)(i=p[f].parentNode).removeChild(p[f]);for(f=l;f<h;++f)i.appendChild(p[l+c[f-l]])})})}var a,u,s,i}r.addEventListener("DOMContentLoaded",function(){for(var n=r.getElementsByClassName("tg"),t=0;t<L(n);++t)try{s(n[t])}catch(n){}})}(document);</script>
                                                                <div class="tg-wrap"> <table id="tg-ZWZmw" class="tg"> <thead> <tr> <th class="tg-ixhd">Section</th> <th class="tg-rn6i">Question<br>Number</th> <th class="tg-ixhd">Questions</th> <th class="tg-rn6i">Assumed<br>Answer</th> <th class="tg-rn6i">Correct<br>Answer</th> <th class="tg-rn6i">Time<br>Taken<br>in minutes</th> <th class="tg-rn6i">Time<br>Taken<br>in Seconds</th> </tr> </thead> <tbody>)";

    std::unordered_map<std::string, std::string> validity_table_row_boiler_plates = {
        {"nv", "<tr><td class=\"tg-d7tt\">$(section_name)</td><td class=\"tg-2p7n\">$(question_number)</td><td class=\"tg-2p7n\"><img src=\"$(question_image_path)\" alt=\"$(alt_question_image)\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-2p7n\">$(assumed_answer)</td><td class=\"tg-2p7n\">$(correct_answer)</td><td class=\"tg-2p7n\">$(time_taken)</td><td class=\"tg-2p7n\">$(time_taken_sec)</td></tr>"},
        {"n", "<tr><td class=\"tg-50kp\">$(section_name)</td><td class=\"tg-fj6v\">$(question_number)</td><td class=\"tg-fj6v\"><img src=\"$(question_image_path)\" alt=\"$(alt_question_image)\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-fj6v\">$(assumed_answer)</td><td class=\"tg-fj6v\">$(correct_answer)</td><td class=\"tg-fj6v\">$(time_taken)</td><td class=\"tg-fj6v\">$(time_taken_sec)</td>"},
        {"nr", "<tr><td class=\"tg-x01x\">$(section_name)</td><td class=\"tg-a6kj\">$(question_number)</td><td class=\"tg-a6kj\"><img src=\"$(question_image_path)\" alt=\"$(alt_question_image)\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-a6kj\">$(assumed_answer)</td><td class=\"tg-a6kj\">$(correct_answer)</td><td class=\"tg-a6kj\">$(time_taken)</td><td class=\"tg-a6kj\">$(time_taken_sec)</td>"},
        {"arc", "<tr><td class=\"tg-jy6h\">$(section_name)</td><td class=\"tg-x4p3\">$(question_number)</td><td class=\"tg-x4p3\"><img src=\"$(question_image_path)\" alt=\"$(alt_question_image)\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-x4p3\">$(assumed_answer)</td><td class=\"tg-x4p3\">$(correct_answer)</td><td class=\"tg-x4p3\">$(time_taken)</td><td class=\"tg-x4p3\">$(time_taken_sec)</td>"},
        {"ac", "<tr><td class=\"tg-a6xf\">$(section_name)</td><td class=\"tg-4dnv\">$(question_number)</td><td class=\"tg-4dnv\"><img src=\"$(question_image_path)\" alt=\"$(alt_question_image)\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-4dnv\">$(assumed_answer)</td><td class=\"tg-4dnv\">$(correct_answer)</td><td class=\"tg-4dnv\">$(time_taken)</td><td class=\"tg-4dnv\">$(time_taken_sec)</td></tr>"},
        {"ai", "<tr><td class=\"tg-50ki\">$(section_name)</td><td class=\"tg-fj6i\">$(question_number)</td><td class=\"tg-fj6i\"><img src=\"$(question_image_path)\" alt=\"$(alt_question_image)\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-fj6i\">$(assumed_answer)</td><td class=\"tg-fj6i\">$(correct_answer)</td><td class=\"tg-fj6i\">$(time_taken)</td><td class=\"tg-fj6i\">$(time_taken_sec)</td></tr>"},
        {"ari", "<tr><td class=\"tg-x01i\">$(section_name)</td><td class=\"tg-a6ki\">$(question_number)</td><td class=\"tg-a6ki\"><img src=\"$(question_image_path)\" alt=\"$(alt_question_image)\" width=\"$(question_image_width)\" height=\"$(question_image_height)\"></td><td class=\"tg-a6ki\">$(assumed_answer)</td><td class=\"tg-a6ki\">$(correct_answer)</td><td class=\"tg-a6ki\">$(time_taken)</td><td class=\"tg-a6ki\">$(time_taken_sec)</td></tr>"}, 
    };

    std::string validity_summary_boiler_plate = R"(<style type="text/css">.tg td,.tg th{border:1px solid #93a1a1;font-family:Arial,sans-serif;font-size:14px;overflow:hidden;padding:10px 5px;word-break:normal}.tg .tg-8gy3,.tg .tg-b7jv,.tg .tg-xtui{top:-1px;will-change:transform}.tg{border-collapse:collapse;border-color:#93a1a1;border-spacing:0;margin:0 auto}.tg td{background-color:#fdf6e3;color:#002b36}.tg th{background-color:#657b83;color:#fdf6e3;font-weight:400}.tg .tg-t2s5{background-color:red;border-color:inherit;color:#6200c9;font-family:serif!important;font-size:26px;font-weight:700;text-align:center;vertical-align:top}.tg .tg-b7jv{border-color:inherit;font-family:"Lucida Sans Unicode","Lucida Grande",sans-serif!important;font-size:32px;font-weight:700;position:-webkit-sticky;position:sticky;text-align:center;vertical-align:top}.tg .tg-e730,.tg .tg-nvf3,.tg .tg-z56b{font-family:serif!important;font-size:26px;font-weight:700;border-color:inherit;text-align:center;vertical-align:top}.tg .tg-z56b{background-color:#ffffc7}.tg .tg-e730,.tg .tg-nvf3{background-color:#69ee7f}.tg .tg-inn7{background-color:red;border-color:inherit;font-family:"Arial Black",Gadget,sans-serif!important;font-size:22px;text-align:center;vertical-align:top}.tg .tg-gq9z{background-color:#69ee7f;border-color:inherit;color:#6200c9;font-family:serif!important;font-size:26px;font-weight:700;text-align:center;vertical-align:top}.tg .tg-ambt{background-color:red;border-color:inherit;color:#6200c9;font-family:"Arial Black",Gadget,sans-serif!important;font-size:22px;text-align:center;vertical-align:top}.tg .tg-8gy3{border-color:#002b36;font-size:32px;font-weight:700;position:-webkit-sticky;position:sticky;text-align:center;vertical-align:top}.tg .tg-8mql,.tg .tg-hdkt,.tg .tg-rgfe{border-color:inherit;font-family:serif!important;font-size:26px;font-weight:700}.tg .tg-8mql,.tg .tg-rgfe{background-color:#eee8d5;text-align:center;vertical-align:top}.tg .tg-hdkt{background-color:#fd6864;text-align:center;vertical-align:top}.tg .tg-0iwr,.tg .tg-4wfb{font-family:"Arial Black",Gadget,sans-serif!important;font-size:22px;text-align:center;vertical-align:top;border-color:inherit}.tg .tg-4wfb{background-color:#ffffc7}.tg .tg-0iwr{background-color:#69ee7f;color:#6200c9}.tg .tg-xtui{border-color:inherit;font-size:32px;font-weight:700;position:-webkit-sticky;position:sticky;text-align:center;vertical-align:top}.tg .tg-34fi,.tg .tg-caq7,.tg .tg-m4gr,.tg .tg-xjah{font-family:"Arial Black",Gadget,sans-serif!important;font-size:22px;text-align:center;vertical-align:top}.tg .tg-xjah{background-color:#eee8d5;border-color:inherit}.tg .tg-m4gr{background-color:#69ee7f;border-color:inherit}.tg .tg-caq7{background-color:#fd6864;border-color:inherit}.tg .tg-34fi{background-color:#eee8d5}.tg .tg-069e,.tg .tg-0pa9{background-color:red;border-color:inherit;font-family:serif!important;font-size:26px;font-weight:700;text-align:center;vertical-align:top}.tg-sort-header::-moz-selection{background:0 0}.tg-sort-header::selection{background:0 0}.tg-sort-header{cursor:pointer}.tg-sort-header:after{content:'';float:right;margin-top:7px;border-width:0 5px 5px;border-style:solid;border-color:#404040 transparent;visibility:hidden}.tg-sort-header:hover:after{visibility:visible}.tg-sort-asc:after,.tg-sort-asc:hover:after,.tg-sort-desc:after{visibility:visible;opacity:.4}.tg-sort-desc:after{border-bottom:none;border-width:5px 5px 0}@media screen and (max-width:767px){.tg,.tg col{width:auto!important}.tg-wrap{overflow-x:auto;-webkit-overflow-scrolling:touch;margin:auto 0}}</style>
                                                   <script charset="utf-8">var TGSort = window.TGSort || function (n) { "use strict"; function r(n) { return n ? n.length : 0 } function t(n, t, e, o = 0) { for (e = r(n); o < e; ++o)t(n[o], o) } function e(n) { return n.split("").reverse().join("") } function o(n) { var e = n[0]; return t(n, function (n) { for (; !n.startsWith(e);)e = e.substring(0, r(e) - 1) }), r(e) } function u(n, r, e = []) { return t(n, function (n) { r(n) && e.push(n) }), e } var a = parseFloat; function i(n, r) { return function (t) { var e = ""; return t.replace(n, function (n, t, o) { return e = t.replace(r, "") + "." + (o || "").substring(1) }), a(e) } } var s = i(/^(?:\s*)([+-]?(?:\d+)(?:,\d{3})*)(\.\d*)?$/g, /,/g), c = i(/^(?:\s*)([+-]?(?:\d+)(?:\.\d{3})*)(,\d*)?$/g, /\./g); function f(n) { var t = a(n); return !isNaN(t) && r("" + t) + 1 >= r(n) ? t : NaN } function d(n) { var e = [], o = n; return t([f, s, c], function (u) { var a = [], i = []; t(n, function (n, r) { r = u(n), a.push(r), r || i.push(n) }), r(i) < r(o) && (o = i, e = a) }), r(u(o, function (n) { return n == o[0] })) == r(o) ? e : [] } function v(n) { if ("TABLE" == n.nodeName) { for (var a = function (r) { var e, o, u = [], a = []; return function n(r, e) { e(r), t(r.childNodes, function (r) { n(r, e) }) }(n, function (n) { "TR" == (o = n.nodeName) ? (e = [], u.push(e), a.push(n)) : "TD" != o && "TH" != o || e.push(n) }), [u, a] }(), i = a[0], s = a[1], c = r(i), f = c > 1 && r(i[0]) < r(i[1]) ? 1 : 0, v = f + 1, p = i[f], h = r(p), l = [], g = [], N = [], m = v; m < c; ++m) { for (var T = 0; T < h; ++T) { r(g) < h && g.push([]); var C = i[m][T], L = C.textContent || C.innerText || ""; g[T].push(L.trim()) } N.push(m - v) } t(p, function (n, t) { l[t] = 0; var a = n.classList; a.add("tg-sort-header"), n.addEventListener("click", function () { var n = l[t]; !function () { for (var n = 0; n < h; ++n) { var r = p[n].classList; r.remove("tg-sort-asc"), r.remove("tg-sort-desc"), l[n] = 0 } }(), (n = 1 == n ? -1 : +!n) && a.add(n > 0 ? "tg-sort-asc" : "tg-sort-desc"), l[t] = n; var i, f = g[t], m = function (r, t) { return n * f[r].localeCompare(f[t]) || n * (r - t) }, T = function (n) { var t = d(n); if (!r(t)) { var u = o(n), a = o(n.map(e)); t = d(n.map(function (n) { return n.substring(u, r(n) - a) })) } return t }(f); (r(T) || r(T = r(u(i = f.map(Date.parse), isNaN)) ? [] : i)) && (m = function (r, t) { var e = T[r], o = T[t], u = isNaN(e), a = isNaN(o); return u && a ? 0 : u ? -n : a ? n : e > o ? n : e < o ? -n : n * (r - t) }); var C, L = N.slice(); L.sort(m); for (var E = v; E < c; ++E)(C = s[E].parentNode).removeChild(s[E]); for (E = v; E < c; ++E)C.appendChild(s[v + L[E - v]]) }) }) } } n.addEventListener("DOMContentLoaded", function () { for (var t = n.getElementsByClassName("tg"), e = 0; e < r(t); ++e)try { v(t[e]) } catch (n) { } }) }(document)</script>
                                                   <div class="tg-wrap"> <table id="tg-r5lDF" class="tg"> <thead> <tr> <th class="tg-b7jv">Section</th> <th class="tg-b7jv">Correct</th> <th class="tg-b7jv">Correct &amp;<br>marked for review</th> <th class="tg-b7jv">Incorrect</th> <th class="tg-b7jv">Incorrect &amp;<br>marked for review</th> <th class="tg-xtui">Not<br>Answered</th> <th class="tg-xtui">Not<br>Visited</th> <th class="tg-8gy3">Total</th> </tr> </thead> <tbody>)";
    std::string validity_summary_replaceable_text_for_section = R"(<tr> <td class="tg-xjah">$(section_name)</td> <td class="tg-m4gr">$(section_AC)</td> <td class="tg-0iwr">$(section_ARC)</td> <td class="tg-inn7">$(section_AI)</td> <td class="tg-ambt">$(section_ARI)</td> <td class="tg-caq7">$(section_NA)</td> <td class="tg-4wfb">$(section_NV)</td> <td class="tg-34fi">$(TS)</td> </tr>)";
    std::string validity_summary_replaceable_text_for_total = R"( <tr> <td class="tg-8mql">Total</td> <td class="tg-nvf3">$(AC)</td> <td class="tg-gq9z">$(ARC)</td> <td class="tg-0pa9">$(AI)</td> <td class="tg-t2s5">$(ARI)</td> <td class="tg-hdkt">$(NA)</td> <td class="tg-z56b">$(NV)</td> <td class="tg-8mql">$(TM)</td> </tr>)";
    std::string validity_summary_replaceable_text_for_sub_total = R"( <tr> <td class="tg-rgfe">Total</td> <td class="tg-e730" colspan="2">$(CC)</td> <td class="tg-069e" colspan="2">$(II)</td> <td class="tg-rgfe" colspan="2">$(NN)</td> <td class="tg-rgfe">$(TM)</td> </tr> </tbody> </table></div>)";
};
