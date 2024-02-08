#pragma once
#include "basic_csv_functions.h"
#include "test_starting_info.h"
#include <rapidcsv.h>

class htmlGenerator
{
public:
    void create_test_summary(rapidcsv::Document doc, std::string html_file_name, const test_info test_starting_data);

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
};
